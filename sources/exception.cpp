#include "../headers/exception.h"

nlohmann::json load_json_verified(const std::string& filename) {
    std::ifstream f(filename);

    if (!f.is_open()) {
        throw file_integrity_error(filename, "File not found or access denied.");
    }

    nlohmann::json j;
    try {
        f >> j;
    } catch (const nlohmann::json::parse_error& e) {
        throw file_integrity_error(filename, "Json syntax error. Details: " + std::string(e.what()));
    }

    if (j.is_null()) {
        throw file_integrity_error(filename, "File content is empty or null.");
    }

    return j;
}

int get_verified_input(int min, int max) {
    int choice;

    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw invalid_input_error("Please enter a valid number! ");
    }

    if (choice < min || choice > max) {
        throw invalid_input_error("Choice must be between " + std::to_string(min) + " and " + std::to_string(max) + ".");
    }

    return choice;
}