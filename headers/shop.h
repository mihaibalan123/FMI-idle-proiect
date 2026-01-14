#ifndef OOP_SHOP_H
#define OOP_SHOP_H
#include <vector>
#include "book.h"
#include "drink.h"
#include "cheating_sheet.h"
#include "gadget.h"

class shop {
    std::vector<book *> books_list;
    std::vector<drink *> drinks_list;
    std::vector<cheating_sheet *> cheating_sheets_list;
    std::vector<gadget *> gadgets_list;

    static void sell_item(player &p);

    static void enter_gas_station(player &p);

public:
    shop()
        : books_list(item::load_items<book>("books.json")),
          drinks_list(item::load_items<drink>("drinks.json")),
          cheating_sheets_list(item::load_items<cheating_sheet>("cheating_sheets.json")),
          gadgets_list(item::load_items<gadget>("gadgets.json")) {
    }

    ~shop() {
        for (auto p: books_list) delete p;
        for (auto p: drinks_list) delete p;
        for (auto p: cheating_sheets_list) delete p;
        for (auto p: gadgets_list) delete p;
        books_list.clear();
        drinks_list.clear();
        cheating_sheets_list.clear();
        gadgets_list.clear();
    }

    void run(player &p) const;
};


#endif //OOP_SHOP_H
