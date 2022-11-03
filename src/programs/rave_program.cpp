#include "rave_program.h"

#include "../lightstrip.h"
#include "../pwm.h"

#include <thread>
#include <chrono>

void rave_program::loop() {
    int r, g, b;

    switch(m_color_status) {
        case color_status::RED:
            r = 255;
            g = 0;
            b = 0;
            m_color_status = color_status::ORANGE;
            break;
        case color_status::ORANGE:
            r = 255;
            g = 69;
            b = 0;
            m_color_status = color_status::YELLOW;
            break;
        case color_status::YELLOW:
            r = 255;
            g = 255;
            b = 0;
            m_color_status = color_status::GREEN;
            break;
        case color_status::GREEN:
            r = 0;
            g = 255;
            b = 0;
            m_color_status = color_status::BLUE;
            break;
        case color_status::BLUE:
            r = 0;
            g = 0;
            b = 255;
            m_color_status = color_status::INDIGO;
            break;
        case color_status::INDIGO:
            r = 75;
            g = 0;
            b = 130;
            m_color_status = color_status::WHITE;
            break;
        case color_status::WHITE:
            r = 255;
            g = 255;
            b = 255;
            m_color_status = color_status::RED;
            break;
        default:
            r = 0;
            g = 0;
            b = 0;
            m_color_status = color_status::RED;
            break;
    }

    int delay = static_cast<int>(300 * (1 / m_speed));
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    m_light_strip->write_pwm(r, g, b);
}

void rave_program::on_interrupt() {}
void rave_program::on_stop() {}