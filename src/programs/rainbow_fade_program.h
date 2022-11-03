#pragma once

#include "program.h"

#include <vector>

class rainbow_fade_program : public program {
    public:
        rainbow_fade_program(light_strip* light_strip)
            : program(light_strip, "Rainbow Fade", "Fade between several different colors.") ,
            m_r(0),
            m_g(0),
            m_b(0),
            m_color_status(color_status::RED)
        {}

        ~rainbow_fade_program() {}

        virtual void loop();
        virtual void on_interrupt();
        virtual void on_stop();

    private:
        int m_r;
        int m_b;
        int m_g;

        bool set_color(int r, int g, int b);

        enum class color_status {
            RED,
            ORANGE,
            YELLOW,
            GREEN,
            CYAN,
            BLUE,
            PURPLE
        } m_color_status;
};