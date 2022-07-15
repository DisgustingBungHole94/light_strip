#include "lightstrip.h"

#include <homecontroller/exception.h>
#include <iostream>

#include "console.h"
#include "pwm.h"

LightStrip::LightStrip()
    : m_realR(0.0f), m_realG(0.0f), m_realB(0.0f)
{}
LightStrip::~LightStrip() {}

bool LightStrip::start() {
     try {
        if (!PWM::init("pwm-test")) {
            console::csh("Failed to start PWM!");
            return false;
        }
        PWM::resetPins();
        console::log("PWM started.");

        ProgramManager::init(this);

        console::log("Login required!");
        std::string username = console::input("Username: ");
        std::string password = console::input("Password: ");

        init();
        console::log("Initialized device services.");

        login(username, password);
        console::log("Logged in.");
        
        std::string id = "baaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
        //std::string id = device.registerDevice("Test", "light_strip");
        console::log("Using device ID: " + id);

        console::log("Starting device loop!");

        m_updateColorThread = std::thread([this]{ updateColor(); });

        setId(id);
        run();
    } catch(hc::Exception& e) {
        console::csh("Error: " + e.what());
        return false;
    }

    return true;
}

void LightStrip::cleanup() {
    m_running = false;

    m_updateColorThread.join();

    stop();
    PWM::stop();

    if (m_info.m_programRunning) {
        stopProgram();
    }
}

std::vector<uint8_t> LightStrip::onMessage(const std::vector<uint8_t>& msg) {
    std::vector<uint8_t> response;

    switch(msg[0]) {
        case 0x00:
            response.push_back(m_realR);
            response.push_back(m_realG);
            response.push_back(m_realB);
            break;
        case 0x01:
            if (msg.size() < 4) {
                response.push_back(0x02);
                break;
            }

            response.push_back(setColor(msg[1], msg[2], msg[3]));
            break;
        case 0x02:
            if (msg.size() < 3) {
                response.push_back(0x02);
                break;
            }

            {
                int speedVal = (msg[2] << 8) | (msg[1] & 0xFF);
                response.push_back(setSpeed(speedVal));
            }
            break;
        case 0x03:
            if (msg.size() < 2) {
                response.push_back(0x02);
                break;
            }

            response.push_back(startProgram(msg[1]));
            break;
        case 0x04:
            response.push_back(stopProgram());
            break;
        case 0x05:
            response.push_back(interruptProgram(msg));
            break;
        default:
            response.push_back(0x01);
            break;
    }

    return response;
}

void LightStrip::writePWM(uint8_t r, uint8_t g, uint8_t b) {
    PWM::analogWrite(PWM::PWM_PIN_R, r / 255.0f);
    PWM::analogWrite(PWM::PWM_PIN_G, g / 255.0f);
    PWM::analogWrite(PWM::PWM_PIN_B, b / 255.0f);

    m_realR = r;
    m_realG = g;
    m_realB = b;
}

void LightStrip::updateColor() {
    while(m_running) {
        if (!m_info.m_programRunning) {
            if (m_realR == m_info.m_r && m_realG == m_info.m_g && m_realB == m_info.m_b) {
                continue;
            }

            if (m_realR < m_info.m_r) {
                m_realR += 10;
                if (m_realR > m_info.m_r) m_realR = m_info.m_r;
            }
            if (m_realR > m_info.m_r) {
                m_realR -= 10;
                if (m_realR < m_info.m_r) m_realR = m_info.m_r;
            }
            
            if (m_realG < m_info.m_g) {
                m_realG += 10;
                if (m_realG > m_info.m_g) m_realG = m_info.m_g;
            }
            if (m_realG > m_info.m_g) {
                m_realG -= 10;
                if (m_realG < m_info.m_g) m_realG = m_info.m_g;
            }
            
            if (m_realB < m_info.m_b) {
                m_realB += 10;
                if (m_realB > m_info.m_b) m_realB = m_info.m_b;
            }
            if (m_realB > m_info.m_b) {
                m_realB -= 10;
                if (m_realB < m_info.m_b) m_realB = m_info.m_b;
            }

            PWM::analogWrite(PWM::PWM_PIN_R, m_realR / 255.0f);
            PWM::analogWrite(PWM::PWM_PIN_G, m_realG / 255.0f);
            PWM::analogWrite(PWM::PWM_PIN_B, m_realB / 255.0f);

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

uint8_t LightStrip::setColor(uint8_t r, uint8_t g, uint8_t b) {
    if (m_info.m_programRunning) {
        stopProgram();
    }

    m_info.m_r = r;
    m_info.m_g = g;
    m_info.m_b = b;

    std::string colorStr = std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b);
    console::log("Color updated: " + colorStr);

    return 0x00;
}

uint8_t LightStrip::setSpeed(int speed) {
    if (speed < 25 || speed > 400) {
        return 0x03;
    }

    m_info.m_speed = speed / 100.0f;

    if (m_info.m_programRunning) {
        m_info.m_program->setSpeed(m_info.m_speed);
    }

    console::log("Speed updated: " + std::to_string(speed) + "%");

    return 0x00;
}

uint8_t LightStrip::startProgram(uint8_t id) {
    std::unique_ptr<Program> program = ProgramManager::getProgram(id);
    if (program == nullptr) {
        return 0x04;
    }

    if (m_info.m_programRunning) {
        stopProgram();
    }

    m_info.m_program = std::move(program);
    m_info.m_program->setSpeed(m_info.m_speed);

    PWM::resetPins();

    m_info.m_programThread = std::thread(&Program::run, m_info.m_program.get());
    m_info.m_programRunning = true;

    console::log("Program [" + m_info.m_program->getTitle() + "] started!");

    return 0x00;
}

uint8_t LightStrip::stopProgram() {
    if (!m_info.m_programRunning) {
        return 0x05;
    }

    m_info.m_program->stop();
    m_info.m_programThread.join();
    m_info.m_programRunning = false;

    PWM::resetPins();

    m_info.m_r = 0;
    m_info.m_g = 0;
    m_info.m_b = 0;

    m_realR = 0;
    m_realG = 0;
    m_realB = 0;

    console::log("Program [" + m_info.m_program->getTitle() + "] stopped.");

    m_info.m_program.reset();

    return 0x00;
}

uint8_t LightStrip::interruptProgram(const std::vector<uint8_t> data) {
    if (!m_info.m_programRunning) {
        return 0x05;
    }

    m_info.m_program->interrupt(data);

    return 0x00;
}