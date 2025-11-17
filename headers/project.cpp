#include "project.h"

inline std::istream& operator>>(std::istream& is, project& t) {
    is >> t.proj_name>> t.market_price>> t.if_bought_cashback;
    return is;
}

std::ostream& operator<<(std::ostream& os, const project& p) {
    os << p.get_name() << " (price of his project is: " << p.get_price() << " with cashback: " << p.get_cashback() << ")" << "\n";
    return os;
}