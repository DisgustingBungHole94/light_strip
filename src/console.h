#pragma once

#include <string>

namespace console {
    static const std::string MASTER_PREFIX = "[LightStrip]";

    static const std::string LOG_PREFIX = "[LOG]";
    static const std::string ERR_PREFIX = "[ERROR]";
    static const std::string CSH_PREFIX = "[FATAL]";

    static const std::string IN_PREFIX = "[INPUT]";

    extern void log(const std::string& msg, bool nl = true);
    extern void err(const std::string& msg, bool nl = true);
    extern void csh(const std::string& msg, bool nl = true);

    extern void print(const std::string& prefix, const std::string& msg, bool nl = true);
    extern std::string timestamp();

    extern std::string input(const std::string& prompt);
}