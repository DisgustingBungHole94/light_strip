#include "lightstrip.h"

#include <iostream>

int main() {
    LightStrip device;
    int ec;

    if (!device.start()) {
        ec = -1;
        std::cout << "Exited with non-zero status code." << std::endl;
    } else {
        ec = 0;
        std::cout << "Exited with zero status code." << std::endl;
    }

    device.cleanup();

    return ec;
}