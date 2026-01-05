#ifndef OOP_SHOP_H
#define OOP_SHOP_H
#include <vector>
#include "book.h"
#include "drink.h"
#include "cheating_sheet.h"

class shop {
    std::vector<book*> books_list;
    std::vector<drink*> drinks_list;
    std::vector<cheating_sheet*> cheating_sheets_list;
    static void sell_item(player& p);
public:
    shop() = default;
    ~shop (){
        for (auto p : books_list) delete p;
        for (auto p : drinks_list) delete p;
        for (auto p : cheating_sheets_list) delete p;
        books_list.clear();
        drinks_list.clear();
        cheating_sheets_list.clear();
    }
    void start();
    void run(player& p) const;
};


#endif //OOP_SHOP_H