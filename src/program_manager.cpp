#include "program_manager.h"

#include "programs/rainbow_fade_program.h"
#include "programs/rave_program.h"

#include <iostream>
#include "console.h"

std::map<uint8_t, ProgramInfo> ProgramManager::_programs;
LightStrip* ProgramManager::_device = nullptr;

void ProgramManager::init(LightStrip* device) {
    _device = device;

    // register programs here
    _programs.insert(std::make_pair(0x00, ProgramInfo(
        []() -> Program* { return new RainbowFadeProgram(); },
        "Rainbow Fade",
        "Fade between the colors of the rainbow."
    )));

    _programs.insert(std::make_pair(0x01, ProgramInfo(
        []() -> Program* { return new RaveProgram(); },
        "Rave",
        "Flash between several different colors."
    )));

    ////

    console::log("Loaded programs: ", false);
    for(auto& x : _programs) {
        std::cout << "[" << x.second.m_title << "] ";
    }
    std::cout << std::endl;
}

std::unique_ptr<Program> ProgramManager::getProgram(uint8_t id) {
    std::unique_ptr<Program> program;

    auto mit = _programs.find(id);
    if (mit != _programs.end()) {
        program.reset(mit->second.m_constructor());

        program->setDevice(_device);

        program->setTitle(mit->second.m_title);
        program->setDescription(mit->second.m_description);

        return std::move(program);
    }

    return nullptr;
}