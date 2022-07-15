#pragma once

#include <fstream>
#include <memory>
#include <mutex>
#include <string>

class PWM {
    public:
        static bool init(const std::string& pwmFile);

        static void analogWrite(unsigned int pin, float value);

        static void resetPins();
        static void releasePin(unsigned int pin);

        static void stop();

        static const unsigned int PWM_PIN_R;
        static const unsigned int PWM_PIN_G;
        static const unsigned int PWM_PIN_B;


    private:
        static std::mutex m_mutex;

        static std::ofstream _file;
        static bool _init;
};