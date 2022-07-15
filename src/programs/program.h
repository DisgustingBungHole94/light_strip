#pragma once

#include <string>
#include <vector>

class LightStrip;

enum class ProgramStatus {
    RUNNING, INTERRUPT, STOPPED
};

class Program {
    public:
        Program();
        virtual ~Program();

        void run();
        void interrupt(const std::vector<uint8_t>& data);
        void stop();

        virtual void loop();
        virtual void onInterrupt();
        virtual void onStop();

        void setSpeed(float speed) { m_speed = speed; }
        float getSpeed() { return m_speed; }

        void setTitle(const std::string& title) { m_title = title; }
        const std::string& getTitle() { return m_title; }

        void setDescription(const std::string& description) { m_description = description; }
        const std::string& getDescription() { return m_description; }

        void setDevice(LightStrip* device) { m_device = device; }

    private:
        ProgramStatus m_status;

        std::string m_name;
        std::string m_title;
        std::string m_description;

    protected:
        LightStrip* m_device;

        float m_speed;

        std::vector<uint8_t> m_interruptData;
};