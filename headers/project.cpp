#include "project.h"

project::project(const nlohmann::json& j) :
    name(j["name"]),
    price(j["price"]),
    cashback(j["cashback"])
{}

inline std::istream& operator>>(std::istream& is, project& t) {
    is >> t.name>> t.price>> t.cashback;
    return is;
}

std::ostream& operator<<(std::ostream& os, const project& p) {
    os << p.get_name() << " (price of his project is: " << p.get_price() << " with cashback: " << p.get_cashback() << ")" << "\n";
    return os;
}