#include "projects.h"

inline std::istream& operator>>(std::istream& is, projects& t) {
    is >> t.name>> t.price>> t.cashback;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const projects& t) {
    os << t.name<< " "<< t.price<< " "<< t.cashback;
    return os;
}