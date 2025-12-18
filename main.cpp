#include <iostream>
#include "headers/menu.h"
#include "headers/exception.h"

int main() {
    try {
        menu main_menu;
        main_menu.run();
    }
    catch (const file_integrity_error& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "\n" << e.what() << "\n";
        return 1;
    }
    return 0;
}