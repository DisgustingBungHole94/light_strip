#pragma once

#include "program.h"

#include <vector>

class RaveProgram : public Program {
    public:
        RaveProgram();
        ~RaveProgram();

        virtual void loop();
        virtual void onInterrupt();
        virtual void onStop();

    private:
        enum class ColorStatus {
            RED,
            ORANGE,
            YELLOW,
            GREEN,
            BLUE,
            INDIGO,
            WHITE
        };

        ColorStatus m_colorStatus;
};