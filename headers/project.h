#ifndef OOP_PROJECTS_H
#define OOP_PROJECTS_H
#include<string>
#include <fstream>
#include <nlohmann/json.hpp>

class project {
    std::string proj_name;
    float market_price, if_bought_cashback;
public:
    project(std::string name, float price, float cashback): proj_name(std::move(name)), market_price(price), if_bought_cashback(cashback){};
    project() : market_price(0.0f), if_bought_cashback(0.0f){};
    ~project() = default;

    [[nodiscard]] const std::string& get_name() const;
    [[nodiscard]] float get_price() const;
    [[nodiscard]] float get_cashback() const;

    static std::vector<project> load_projects();

    friend std::istream &operator>>(std::istream& is, project& t);
    friend std::ostream& operator<<(std::ostream& os, const project& t);
};


#endif //OOP_PROJECTS_H