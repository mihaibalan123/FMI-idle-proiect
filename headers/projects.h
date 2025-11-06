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

    ~projects()=default;

    projects(const projects& other) = default;

    projects& operator=(const projects& other) {
        if (this == &other) return *this;
        this->name = other.name;
        this->price = other.price;
        this->cashback = other.cashback;
        return *this;
    }

    friend std::istream &operator>>(std::istream& is, projects& t);
    friend std::ostream& operator<<(std::ostream& os, const projects& t);

    [[nodiscard]] std::string get_project_name() const {
        return name;
    }

    [[nodiscard]] float get_project_price() const {
        return price;
    }

};


#endif //OOP_PROJECTS_H