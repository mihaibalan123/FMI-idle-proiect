#include "project.h"

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
    os << p.get_name() << " (price of his project is: " << p.get_price() << " with cashback: " << p.get_cashback() << ")" << "\n";
    return os;
}