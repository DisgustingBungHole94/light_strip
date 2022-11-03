#include "lightstrip.h"

#include <homecontroller/net/endpoints.h>
#include <homecontroller/util/logger.h>
#include <homecontroller/exception/exception.h>

#include <iostream>
#include <unordered_map>
#include <functional>

#include "pwm.h"

#include "programs/rainbow_fade_program.h"
#include "programs/rave_program.h"

bool light_strip::start() {
     try {
        if (!PWM::init("pwm-test")) {
            hc::util::logger::csh("failed to start PWM");
            return false;
        }

        PWM::reset_pins();

        hc::util::logger::log("PWM started.");

        hc::util::logger::log("login required...");
        std::string username = hc::util::logger::input("username: ");
        std::string password = hc::util::logger::input("password: ");

        m_api.connect();

        m_api.login(username, password);
        m_api.register_device("Josh's Lights", "light_strip");
    } catch(hc::exception& e) {
        m_api.disconnect();

        hc::util::logger::csh("error: " + std::string(e.what()) + " (" + std::string(e.func()) + ")");
        return false;
    }

    m_running = true;

    std::thread color_update_thread( [this]() { update_color(); } );

    loop();

    color_update_thread.join();

    return true;
}

void light_strip::shutdown() {
    hc::util::logger::log("shutting down...");

    m_running = false;

    m_api.disconnect();

    PWM::stop();

    if (m_info.m_program_running) {
        stop_program();
    }
}

void light_strip::loop() {
    while(m_running) {
        try {
            std::string data = m_api.get_conn_ptr()->recv();
            if (m_api.get_conn_ptr()->closed()) {
                hc::util::logger::err("socket closed");
                shutdown();
                break;
            }

            hc::util::logger::log("received: " + data);
        } catch(hc::exception& e) {
            if (m_running) {
                hc::util::logger::csh("error: " + std::string(e.what()));
                shutdown();
            }
        }
    }

    /*switch(msg[0]) {
        case 0x00:
            response.push_back(m_real_r);
            response.push_back(m_real_g);
            response.push_back(m_real_b);
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

    return response;*/
}

void light_strip::write_pwm(uint8_t r, uint8_t g, uint8_t b) {
    PWM::analog_write(PWM::PWM_PIN_R, r / 255.0f);
    PWM::analog_write(PWM::PWM_PIN_G, g / 255.0f);
    PWM::analog_write(PWM::PWM_PIN_B, b / 255.0f);

    m_real_r = r;
    m_real_g = g;
    m_real_b = b;
}

void light_strip::update_color() {
    while(m_running) {
        if (!m_info.m_program_running) {
            if (m_real_r == m_info.m_r && m_real_g == m_info.m_g && m_real_b == m_info.m_b) {
                continue;
            }

            if (m_real_r < m_info.m_r) {
                m_real_r += 10;
                if (m_real_r > m_info.m_r) m_real_r = m_info.m_r;
            }
            if (m_real_r > m_info.m_r) {
                m_real_r -= 10;
                if (m_real_r < m_info.m_r) m_real_r = m_info.m_r;
            }
            
            if (m_real_g < m_info.m_g) {
                m_real_g += 10;
                if (m_real_g > m_info.m_g) m_real_g = m_info.m_g;
            }
            if (m_real_g > m_info.m_g) {
                m_real_g -= 10;
                if (m_real_g < m_info.m_g) m_real_g = m_info.m_g;
            }
            
            if (m_real_b < m_info.m_b) {
                m_real_b += 10;
                if (m_real_b > m_info.m_b) m_real_b = m_info.m_b;
            }
            if (m_real_b > m_info.m_b) {
                m_real_b -= 10;
                if (m_real_b < m_info.m_b) m_real_b = m_info.m_b;
            }

            PWM::analog_write(PWM::PWM_PIN_R, m_real_r / 255.0f);
            PWM::analog_write(PWM::PWM_PIN_G, m_real_g / 255.0f);
            PWM::analog_write(PWM::PWM_PIN_B, m_real_b / 255.0f);

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

uint8_t light_strip::set_color(uint8_t r, uint8_t g, uint8_t b) {
    if (m_info.m_program_running) {
        stop_program();
    }

    m_info.m_r = r;
    m_info.m_g = g;
    m_info.m_b = b;

    std::string colorStr = std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b);
    hc::util::logger::log("color updated: " + colorStr);

    return 0x00;
}

uint8_t light_strip::set_speed(int speed) {
    if (speed < 25 || speed > 400) {
        return 0x03;
    }

    m_info.m_speed = speed / 100.0f;

    if (m_info.m_program_running) {
        m_info.m_program->set_speed(m_info.m_speed);
    }

    hc::util::logger::log("speed updated: " + std::to_string(speed) + "%");

    return 0x00;
}

uint8_t light_strip::start_program(uint8_t id) {
    static const std::unordered_map<uint8_t, std::function<program*()>> programs = {
        { 0x00, [this]() { return new rainbow_fade_program(this); } },
        { 0x01, [this]() { return new rave_program(this); } }
    };

    std::unique_ptr<program> program_ptr = nullptr;

    auto mit = programs.find(id);
    if (mit == programs.end()) {
        return 0x04;
    }

    program_ptr.reset(mit->second());

    if (m_info.m_program_running) {
        stop_program();
    }

    m_info.m_program = std::move(program_ptr);
    m_info.m_program->set_speed(m_info.m_speed);

    PWM::reset_pins();

    m_info.m_program_thread = std::thread(&program::run, m_info.m_program.get());
    m_info.m_program_running = true;

    hc::util::logger::log("program [" + m_info.m_program->get_title() + "] started!");

    return 0x00;
}

uint8_t light_strip::stop_program() {
    if (!m_info.m_program_running) {
        return 0x05;
    }

    m_info.m_program->stop();
    m_info.m_program_thread.join();
    m_info.m_program_running = false;

    PWM::reset_pins();

    m_info.m_r = 0;
    m_info.m_g = 0;
    m_info.m_b = 0;

    m_real_r = 0;
    m_real_g = 0;
    m_real_b = 0;

    hc::util::logger::log("program [" + m_info.m_program->get_title() + "] stopped.");

    m_info.m_program.reset();

    return 0x00;
}

uint8_t light_strip::interrupt_program(const std::vector<uint8_t> data) {
    if (!m_info.m_program_running) {
        return 0x05;
    }

    m_info.m_program->interrupt(data);

    return 0x00;
}