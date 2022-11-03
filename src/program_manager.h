#pragma once

#include <string>
#include <memory>
#include <map>
#include <functional>

#include "programs/program.h"

class light_strip;

struct ProgramInfo {
    ProgramInfo(std::function<program*()> constructor, std::string title, std::string description) 
        : m_constructor(constructor), m_title(title), m_description(description)
    {}

    std::function<program*()> m_constructor;

    std::string m_title;
    std::string m_description;
};

class ProgramManager {
    public:
        static void init(light_strip* device);
        static std::unique_ptr<program> getProgram(uint8_t id);

        //static const std::map<std::string, ProgramInfo>& getProgramMap() { return _programs; }
    private:
        static light_strip* _device;

        static std::map<uint8_t, ProgramInfo> _programs;
};