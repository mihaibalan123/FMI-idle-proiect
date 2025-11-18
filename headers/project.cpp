#include "project.h"

const std::string& project::get_name() const {
    return proj_name;
}

float project::get_price() const {
    return market_price;
}

float project::get_cashback() const {
    return if_bought_cashback;
}

std::vector<project> project::load_projects() {

    std::vector<project> projects_list;
    std::ifstream f1("projects.json");
    nlohmann::json data1 = nlohmann::json::parse(f1);

    for (const auto &i_project: data1) {
        project temp_project(
            i_project["name"],
            i_project["price"],
            i_project["cashback"]
        );
        projects_list.push_back(temp_project);
    }
    return projects_list;
}

inline std::istream& operator>>(std::istream& is, project& t) {
    is >> t.proj_name>> t.market_price>> t.if_bought_cashback;
    return is;
}

std::ostream& operator<<(std::ostream& os, const project& p) {
    os << p.proj_name << " (price of his project is: " << p.market_price << " with cashback: " << p.if_bought_cashback << ")" << "\n";
    return os;
}