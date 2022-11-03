#include "lightstrip.h"

#include <iostream>

#include <csignal>
#include <unistd.h>

light_strip lights;

int main() {
    std::signal(SIGINT, [](int s) {
        lights.shutdown();
    });

    if (!lights.start()) {
        std::cout << "exited with non-zero status code." << std::endl;
        return -1;
    }

    std::cout << "exited with zero status code." << std::endl;
    return 0;
}