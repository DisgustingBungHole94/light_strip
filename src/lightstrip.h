#pragma once

#include <homecontroller/device.h>

#include "programs/program.h"
#include "program_manager.h"

#include <thread>

struct LightStripInfo {
    uint8_t m_r = 0.0f;
    uint8_t m_g = 0.0f;
    uint8_t m_b = 0.0f;

    float m_speed = 1.0f;

    bool m_programRunning = false;

    std::unique_ptr<Program> m_program;
    std::thread m_programThread; 
};

class LightStrip : public hc::Device {
    public:
        LightStrip();
        ~LightStrip();

        bool start();
        void cleanup();

        virtual std::vector<uint8_t> onMessage(const std::vector<uint8_t>& msg);

        void writePWM(uint8_t r, uint8_t g, uint8_t b);
    private:
        void updateColor();
        std::thread m_updateColorThread;

        uint8_t setColor(uint8_t r, uint8_t g, uint8_t b);
        uint8_t setSpeed(int speed);

        uint8_t startProgram(uint8_t id);
        uint8_t stopProgram();
        uint8_t interruptProgram(const std::vector<uint8_t> data);

        bool m_running;

        LightStripInfo m_info;

        int m_realR;
        int m_realG;
        int m_realB;
};