#ifndef OOP_PROJECTS_H
#define OOP_PROJECTS_H

#include<iostream>
#include<string>
#include <vector>
#include<fstream>

class projects {
    std::string name;
    float price, cashback;
public:
    projects(std::string name, float price, float cashback): name(std::move(name)), price(price), cashback(cashback){};
    projects() : price(0.0f), cashback(0.0f){};
    ~projects()=default;

    projects(const projects& other): name(other.name), price(other.price), cashback(other.cashback){};

    projects& operator=(const projects& other) {
        if (this == &other) return *this;
        this->name = other.name;
        this->price = other.price;
        this->cashback = other.cashback;
        return *this;
    }

    friend std::istream &operator>>(std::istream& is, projects &t);

    void show_projects() const{
        std::cout<< name <<" "<< "\n";
    }

};

    inline std::istream& operator>>(std::istream& is, projects& t) { // supraincarcarea operatorului
    is >> t.name>> t.price>> t.cashback;
    return is;
    }

    inline std::vector<projects> read_projects(const std::string& filename) {
        std::ifstream f(filename);

        std::vector<projects> projects_list;
        projects temp_project;

        while (f>> temp_project) {
            projects_list.push_back(temp_project);
        }
        f.close();
        return projects_list;
    };


#endif //OOP_PROJECTS_H