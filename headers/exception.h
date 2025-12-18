#ifndef OOP_EXCEPTION_H
#define OOP_EXCEPTION_H

#include <exception>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>


class game_exception : public std::exception {
protected:
    std::string message;

public:
    explicit game_exception(const std::string &msg) : message(msg) {
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }
};

class file_integrity_error : public game_exception {
public:
    file_integrity_error(const std::string &filename, const std::string &details)
        : game_exception("FILE ERROR [" + filename + "]: " + details) {
    }
};

class invalid_input_error : public game_exception {
public:
    explicit invalid_input_error(const std::string &details)
        : game_exception("INPUT ERROR: " + details) {
    }
};

class insufficient_funds_error : public game_exception {
public:
    explicit insufficient_funds_error(const std::string& item_name)
        : game_exception("Transaction Failed: Insufficient funds for '" + item_name + "'.") {}
};

class player_weak_error : public game_exception {
public:
    player_weak_error()
        : game_exception("Combat Prevented: Your health is too low! Heal up first.") {}
};

nlohmann::json load_json_verified(const std::string &filename);

int get_verified_input(int min, int max);


#endif //OOP_EXCEPTION_H
