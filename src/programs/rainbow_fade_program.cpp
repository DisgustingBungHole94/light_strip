#include "rainbow_fade_program.h"

#include "../lightstrip.h"

#include <thread>
#include <chrono>

RainbowFadeProgram::RainbowFadeProgram() 
    : m_r(0), m_g(0), m_b(0), m_colorStatus(ColorStatus::RED)
{}

RainbowFadeProgram::~RainbowFadeProgram() {}

void RainbowFadeProgram::loop() {
    switch(m_colorStatus) {
        case ColorStatus::RED:
            if (setColor(255, 0, 0)) m_colorStatus = ColorStatus::ORANGE;
            break;
        case ColorStatus::ORANGE:
            if (setColor(255, 69, 0)) m_colorStatus = ColorStatus::YELLOW;
            break;
        case ColorStatus::YELLOW:
            if (setColor(255, 255, 0)) m_colorStatus = ColorStatus::GREEN;
            break;
        case ColorStatus::GREEN:
            if (setColor(0, 255, 0)) m_colorStatus = ColorStatus::CYAN;
            break;
        case ColorStatus::CYAN:
            if (setColor(0, 255, 255)) m_colorStatus = ColorStatus::BLUE;
            break;
        case ColorStatus::BLUE:
            if (setColor(0, 0, 255)) m_colorStatus = ColorStatus::PURPLE;
            break;
        case ColorStatus::PURPLE:
            if (setColor(255, 0, 255)) m_colorStatus = ColorStatus::RED;
            break;
        default:
            m_colorStatus = ColorStatus::RED;
            break;
    }

    m_device->writePWM(m_r, m_g, m_b);

    int delay = static_cast<int>(10 * (1 / m_speed));
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}

void RainbowFadeProgram::onInterrupt() {}

void RainbowFadeProgram::onStop() {}

bool RainbowFadeProgram::setColor(int r, int g, int b) {
    if (r == m_r && g == m_g && b == m_b) {
        return true;
    }

    if (r > m_r) m_r += 1;
    if (r < m_r) m_r -= 1;

    if (g > m_g) m_g += 1;
    if (g < m_g) m_g -= 1;

    if (b > m_b) m_b += 1;
    if (b < m_b) m_b -= 1;

    return false;
}