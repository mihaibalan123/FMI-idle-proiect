#ifndef OOP_TEACHERS_H
#define OOP_TEACHERS_H

#include <string>
#include<fstream>

class teachers {
    std::string last_name, first_name;
    int rarity, domain, aura, item_id;
    float health, damage, critical_damage;

public:
    teachers(std::string last_name, std::string  first_name, int rarity, int domain, int aura, int item_id, float health,
        float damage, float critical_damage): last_name(std::move(last_name)),
        first_name(std::move(first_name)), rarity(rarity), domain(domain),aura(aura), item_id(item_id), health(health),
        damage(damage), critical_damage(critical_damage) {};
    teachers(): rarity(0), domain(0), aura(0), item_id(0), health(0), damage(0), critical_damage(0){};

    ~teachers()=default;

    teachers(const teachers& other) = default;//constr de copiere

    teachers& operator=(const teachers& other) {
        if (this == &other) return *this;
        this->last_name = other.last_name;
        this->first_name = other.first_name;
        this->rarity = other.rarity;
        this->domain = other.domain;
        this->aura = other.aura;
        this->item_id = other.item_id;
        this->health = other.health;
        this->damage = other.damage;
        this->critical_damage = other. critical_damage;
        return *this;
    }

    friend std::istream& operator>>(std::istream& is, teachers &t);
    friend std::ostream& operator<<(std::ostream& os, const teachers& t);

    /*
    void show_teachers() const{
        std::cout<< last_name<<" "<< first_name<< "\n"; //doar de test momentan
    }

    void total_damage() const{
         std::cout<< last_name <<" "<< first_name<< " has a total possible damage of "<< damage + (critical_damage / 100 * damage)<<"\n";
    }
    */


};

    inline std::istream& operator>>(std::istream& is, teachers& t) { // supraincarcarea operatorului
    is >> t.last_name>> t.first_name>> t.rarity>> t.domain>> t.aura>> t.health>> t.damage>> t.critical_damage>> t.item_id;
    return is;
}

    inline std::ostream& operator<<(std::ostream& os, const teachers& t) {
        os << t.last_name<< " "<< t.first_name<< " "<< t.rarity<< " "<< t.domain<< " " <<t.aura<< " "
        << t.health<< " "<< t.damage<< " "<< t.critical_damage<< " "<< t.item_id;
        return os;
    }



#endif //OOP_TEACHERS_H