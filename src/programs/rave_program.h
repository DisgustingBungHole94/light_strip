#pragma once

#include "program.h"

#include <vector>

class rave_program : public program {
    public:
        rave_program(light_strip* light_strip) 
            : program(light_strip, "Rave", "Flash quickly between several colors."),
            m_color_status(color_status::RED)
        {}

        ~rave_program() {}

        virtual void loop();
        virtual void on_interrupt();
        virtual void on_stop();

    private:
        enum class color_status {
            RED,
            ORANGE,
            YELLOW,
            GREEN,
            BLUE,
            INDIGO,
            WHITE
        } m_color_status;
};