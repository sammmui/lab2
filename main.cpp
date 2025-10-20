
#include "analyzer.h"
#include <iostream>

int main() {
    try {
        analyzer a;
        a.run_tests();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
