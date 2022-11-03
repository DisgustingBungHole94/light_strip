#pragma once

#include <string>
#include <vector>

class light_strip;

enum class program_status {
    RUNNING, INTERRUPT, STOPPED
};

class program {
    public:
        program(light_strip* light_strip, const std::string& title, const std::string& description) 
            : m_light_strip(light_strip),
            m_title(title),
            m_description(description),
            m_speed(0.0f)
        {}

        ~program() {}

        void run();
        void interrupt(const std::vector<uint8_t>& data);
        void stop();

        virtual void loop();
        virtual void on_interrupt();
        virtual void on_stop();

        void set_speed(float speed) { m_speed = speed; }
        float get_speed() { return m_speed; }

        const std::string& get_title() { return m_title; }
        const std::string& get_description() { return m_description; }

    private:
        program_status m_status;

        std::string m_title;
        std::string m_description;

    protected:
        light_strip* m_light_strip;

        float m_speed;

        std::vector<uint8_t> m_interrupt_data;
};