#include "program.h"

#include "../lightstrip.h"

#include <iostream>

Program::Program() 
    : m_status(ProgramStatus::STOPPED), m_speed(1.0f)
{}

Program::~Program() {}

void Program::run() {
    m_status = ProgramStatus::RUNNING;

    while(m_status != ProgramStatus::STOPPED) {
        if (m_status == ProgramStatus::INTERRUPT) {
            onInterrupt();
            m_status = ProgramStatus::RUNNING;
        }

        loop();
    }

    onStop();
}

void Program::interrupt(const std::vector<uint8_t>& data) {
    m_status = ProgramStatus::INTERRUPT;
    m_interruptData = data;
}

void Program::stop() {
    m_status = ProgramStatus::STOPPED;
    m_device->writePWM(0, 0, 0);
}

void Program::loop() {}
void Program::onInterrupt() {}
void Program::onStop() {}