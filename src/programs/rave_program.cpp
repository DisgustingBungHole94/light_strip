#include "rave_program.h"

#include "../lightstrip.h"
#include "../pwm.h"

#include <thread>
#include <chrono>

RaveProgram::RaveProgram()
    : m_colorStatus(ColorStatus::RED)
{}

RaveProgram::~RaveProgram() {}

void RaveProgram::loop() {
    int r, g, b;

    switch(m_colorStatus) {
        case ColorStatus::RED:
            r = 255;
            g = 0;
            b = 0;
            m_colorStatus = ColorStatus::ORANGE;
            break;
        case ColorStatus::ORANGE:
            r = 255;
            g = 69;
            b = 0;
            m_colorStatus = ColorStatus::YELLOW;
            break;
        case ColorStatus::YELLOW:
            r = 255;
            g = 255;
            b = 0;
            m_colorStatus = ColorStatus::GREEN;
            break;
        case ColorStatus::GREEN:
            r = 0;
            g = 255;
            b = 0;
            m_colorStatus = ColorStatus::BLUE;
            break;
        case ColorStatus::BLUE:
            r = 0;
            g = 0;
            b = 255;
            m_colorStatus = ColorStatus::INDIGO;
            break;
        case ColorStatus::INDIGO:
            r = 75;
            g = 0;
            b = 130;
            m_colorStatus = ColorStatus::WHITE;
            break;
        case ColorStatus::WHITE:
            r = 255;
            g = 255;
            b = 255;
            m_colorStatus = ColorStatus::RED;
            break;
        default:
            r = 0;
            g = 0;
            b = 0;
            m_colorStatus = ColorStatus::RED;
            break;
    }

    int delay = static_cast<int>(300 * (1 / m_speed));
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    m_device->writePWM(r, g, b);
}

void RaveProgram::onInterrupt() {}

void RaveProgram::onStop() {}