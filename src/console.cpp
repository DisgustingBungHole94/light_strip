#include "console.h"

#include <iostream>
#include <sstream>
#include <iomanip>

namespace console {
    void log(const std::string& msg, bool nl) {
        print(LOG_PREFIX, msg, nl);
    }

    void err(const std::string& msg, bool nl) {
        print(ERR_PREFIX, msg, nl);
    }

    void csh(const std::string& msg, bool nl) {
        print(CSH_PREFIX, msg, nl);
    }

    void print(const std::string& prefix, const std::string& msg, bool nl) {
        std::ostringstream ss;

        ss << MASTER_PREFIX << " " << timestamp() << " " << prefix << " " << msg;
        if (nl) ss << std::endl;

        std::cout << ss.str();
    }

    std::string timestamp() {
        auto t = std::time(nullptr);
        
        std::ostringstream ss;
        ss << std::put_time(std::localtime(&t), "%m-%d-%Y %H:%M:%S");

        return ss.str();
    }

    std::string input(const std::string& prompt) {
        print(IN_PREFIX, prompt, false);

        std::string in;
        std::getline(std::cin, in);

        return in;
    }
}