#include "program.h"

#include "../lightstrip.h"

#include <iostream>

void program::run() {
    m_status = program_status::RUNNING;

    while(m_status != program_status::STOPPED) {
        if (m_status == program_status::INTERRUPT) {
            on_interrupt();
            m_status = program_status::RUNNING;
        }

        loop();
    }

    on_stop();
}

void program::interrupt(const std::vector<uint8_t>& data) {
    m_status = program_status::INTERRUPT;
    m_interrupt_data = data;
}

void program::stop() {
    m_status = program_status::STOPPED;
    m_light_strip->write_pwm(0, 0, 0);
}

void program::loop() {}
void program::on_interrupt() {}
void program::on_stop() {}