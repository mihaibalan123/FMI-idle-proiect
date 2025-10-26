#ifndef OOP_MENIU_H
#define OOP_MENIU_H

#include <iostream>
#include <string>
#include <vector>
#include <limits>

class menu {
    std::string title;
    std::string description;
    std::vector<std::string> options;
    static void cin_cleaner(){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
public:
    menu(std::string  title, std::string  description, const std::vector<std::string>& options): title(std::move(title)), description(std::move(description)), options(options) {};
    ~menu() = default;
    void show_options() const {
        std::cout<<"\n"<< title<< "\n";
        std::cout<< description <<"\n";
        int i=1;
        std::cout<< "0." << "Exit" << "\n";
        for (const auto& i_option : options) {
            std::cout<< i << "." << i_option << "\n";
            i++;
        }
    }
    [[nodiscard]] int static choose_option() {
        int number_chosen;
        bool ok_chosen_nmb = false;
        do {
            std::cout<< "Type the number of the selected option :";
            std::cin>> number_chosen;
            if (number_chosen >= 0 && number_chosen <= 6) {
                ok_chosen_nmb=true;
            } else {
                std::cout << "Wrong number option. Try again!";
                cin_cleaner();
                ok_chosen_nmb=false;
            }
        }while (!ok_chosen_nmb);
        return number_chosen;
    }

};


#endif //OOP_MENIU_H