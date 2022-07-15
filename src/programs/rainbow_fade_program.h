#pragma once

#include "program.h"

#include <vector>

class RainbowFadeProgram : public Program {
    public:
        RainbowFadeProgram();
        ~RainbowFadeProgram();

        virtual void loop();
        virtual void onInterrupt();
        virtual void onStop();

    private:
        int m_r;
        int m_b;
        int m_g;

        bool setColor(int r, int g, int b);

        enum class ColorStatus {
            RED,
            ORANGE,
            YELLOW,
            GREEN,
            CYAN,
            BLUE,
            PURPLE
        };

        ColorStatus m_colorStatus;
};