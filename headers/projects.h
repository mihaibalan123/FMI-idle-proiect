#ifndef OOP_PROJECTS_H
#define OOP_PROJECTS_H

#include<string>
#include <utility>

class projects {
    std::string name;
    float price, cashback;
public:
    projects(std::string name, float price, float cashback): name(std::move(name)), price(price), cashback(cashback){};
    projects() : price(0.0f), cashback(0.0f){};
    ~projects()=default;


};


#endif //OOP_PROJECTS_H