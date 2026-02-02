#ifndef ITEM_LOADER_H
#define ITEM_LOADER_H

#include "item.h"
#include <nlohmann/json.hpp>
#include <memory>

class ItemLoader {
public:
    static std::unique_ptr<item> create(const nlohmann::json& json_data);
};

#endif // ITEM_LOADER_H