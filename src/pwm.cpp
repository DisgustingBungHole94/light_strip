#include "pwm.h"

#include "console.h"

const unsigned int PWM::PWM_PIN_R = 27;
const unsigned int PWM::PWM_PIN_G = 22;
const unsigned int PWM::PWM_PIN_B = 23;

std::mutex PWM::m_mutex;

std::ofstream PWM::_file;
bool PWM::_init = false;

bool PWM::init(const std::string& pwmFile) {
    _file.open(pwmFile);
    if (!_file.is_open()) {
        return false;
    }

    _init = true;
    return true;
}

void PWM::analogWrite(unsigned int pin, float value) {
    if (!_init) return;

    std::lock_guard<std::mutex> lock(m_mutex);

    _file << std::to_string(pin) << "=" << std::to_string(value) << "\n";
    _file.flush();

    if (!_file.good()) {
        console::err("Failed to write to pin! (pin " + std::to_string(pin) + ")");
    }
}

void PWM::resetPins() {
    if (!_init) return;

    std::lock_guard<std::mutex> lock(m_mutex);

    _file << "*=0" << "\n";
    _file.flush();

    if (!_file.good()) {
        console::err("Failed to reset pins!");
    }
}

void PWM::releasePin(unsigned int pin) {
    if (!_init) return;

    std::lock_guard<std::mutex> lock(m_mutex);

    _file << "release " << std::to_string(pin) << "\n";
    _file.flush();

    if (!_file.good()) {
        console::err("Failed to release pin! (pin " + std::to_string(pin) + ")");
    }
}

void PWM::stop() {
    if (!_init) return;

    std::lock_guard<std::mutex> lock(m_mutex);

    _file.close();
    if (!_file.good()) {
        console::err("Failed to shutdown PWM!");
    }
}