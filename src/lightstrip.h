#pragma once

#include <homecontroller/net/api/api_request_maker.h>

#include "programs/program.h"

#include <thread>

struct light_strip_info {
    uint8_t m_r = 0;
    uint8_t m_g = 0;
    uint8_t m_b = 0;

    float m_speed = 1.0f;

    bool m_program_running = false;

    std::unique_ptr<program> m_program;
    std::thread m_program_thread; 
};

class light_strip {
    public:
        light_strip()
            : m_running(false),
            m_real_r(0),
            m_real_g(0),
            m_real_b(0)
        {}

        ~light_strip() {}

        bool start();
        void loop();
        void shutdown();

        void write_pwm(uint8_t r, uint8_t g, uint8_t b);
    private:
        void update_color();

        uint8_t set_color(uint8_t r, uint8_t g, uint8_t b);
        uint8_t set_speed(int speed);

        uint8_t start_program(uint8_t id);
        uint8_t stop_program();
        uint8_t interrupt_program(const std::vector<uint8_t> data);

        hc::net::api::api_request_maker m_api;

        bool m_running;

        light_strip_info m_info;

        int m_real_r;
        int m_real_g;
        int m_real_b;
};