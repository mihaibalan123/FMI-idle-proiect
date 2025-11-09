#include "projects.h"

inline std::istream& operator>>(std::istream& is, projects& t) {
    is >> t.name>> t.price>> t.cashback;
    return is;
}

std::ostream& operator<<(std::ostream& os, const projects& p) {
    os << p.get_name() << " (price of his project is: " << p.get_price() << "with cashback: " << p.get_cashback() << ")";
    return os;
}