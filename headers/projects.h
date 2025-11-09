#ifndef OOP_PROJECTS_H
#define OOP_PROJECTS_H

#include<iostream>
#include<string>

class projects {
    std::string name;
    float price, cashback;
public:
    projects(std::string name, float price, float cashback): name(std::move(name)), price(price), cashback(cashback){};
    projects() : price(0.0f), cashback(0.0f){};

    [[nodiscard]] const std::string& get_name() const {
        return name;
    }

    [[nodiscard]] float get_price() const {
        return price;
    }

    [[nodiscard]] float get_cashback() const {
        return cashback;
    }

    friend std::istream &operator>>(std::istream& is, projects& t);
    friend std::ostream& operator<<(std::ostream& os, const projects& t);

};


#endif //OOP_PROJECTS_H