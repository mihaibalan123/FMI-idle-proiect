#include "player.h"
#include "teacher.h"
#include "book.h"
#include "cheating_sheet.h"
#include "drink.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

inline void to_json(nlohmann::json& j, const player& p) {
    j = nlohmann::json{
                        {"name", p.get_name()},
                        {"password", p.get_password()},
                        {"conquer_domain", p.get_conquer_domain()},
                        {"currency1", p.get_currency1()},
                        {"currency2", p.get_currency2()},
                        {"health", p.get_health()},
                        {"damage", p.get_damage()},
                        {"project_id", p.get_project_id()},
                        {"project_levels", p.get_project_levels()},
                        {"defeated_domains", p.get_defeated_domains()},
                        {"last_login_timestamp", p.get_last_login_timestamp()}
    };
}

const std::string& player::get_password() const {
    return password;
}

const std::vector<int>& player::get_project_id() const {
    return project_id;
}

float player::get_health() const {
    return health;
}

float player::get_currency1() const {
    return currency1;
}

int player::get_conquer_domain() const {
    return current_target_domain_id;
}

const std::string& player::get_name() const {
    return username;
}

float player::get_damage() const {
    return damage;
}

const std::vector<int>& player::get_defeated_domains() const {
    return defeated_domains;
}

[[nodiscard]] const std::vector<int>& player::get_project_levels() const {
    return project_levels;
}

[[nodiscard]] float player::get_currency2() const {
    return currency2;
}

long long player::get_last_login_timestamp() const {
    return last_login_timestamp;
}

void player::set_currency1(float value) {
    currency1 = value;
}

void player::set_currency2(float value) {
    currency2 = value;
}

void player::set_health(float value) {
    health = value;
}

void player::set_last_login_timestamp(long long timestamp) {
    last_login_timestamp = timestamp;
}

std::vector<player> player::load_players() {

    std::vector<player> players_list;
    std::ifstream f2("players.json");
    nlohmann::json data2 = nlohmann::json::parse(f2);

    for (auto &i_player: data2) {
        std::vector<std::unique_ptr<item>> temp_inventory;
        if (i_player.contains("inventory")) {
            for (const auto& item_json : i_player["inventory"]) {
                std::string type = item_json.value("type", "unknown");
                std::string i_name = item_json.value("name", "Unknown Item");
                std::string i_desc = item_json.value("description", "No desc");
                std::string i_rarity = item_json.value("rarity", "Common");
                float i_price = item_json.value("price", 0.0f);
                bool i_consumable = item_json.value("consumable", false);

                if (type == "book") {
                    float dmg = item_json.value("damage_bonus", 0.0f);
                    temp_inventory.push_back(std::make_unique<book>(
                        i_name, i_desc, i_rarity, i_price, i_consumable, dmg
                    ));
                }
                else if (type == "drink") {
                    float hp = item_json.value("health_restore", 0.0f);
                    temp_inventory.push_back(std::make_unique<drink>(
                        i_name, i_desc, i_rarity, i_price, i_consumable, hp
                    ));
                }
                else if (type == "cheating_sheet") {
                    int boost = item_json.value("project_boost", 0);
                    float chance = item_json.value("success_chance", 0.0f);
                    float risk = item_json.value("risk_damage", 0.0f);
                    temp_inventory.push_back(std::make_unique<cheating_sheet>(
                        i_name, i_desc, i_rarity, i_price, i_consumable, boost, chance, risk
                    ));
                }
            }
        }

        player temp_player(
            i_player.value("name", std::string{""}),
            i_player.value("password", std::string{""}),
            i_player.value("conquer_domain", 0),
            i_player.value("currency1", 0.0f),
            i_player.value("currency2", 0.0f),
            i_player.value("health", 0.0f),
            i_player.value("damage", 0.0f),
            i_player.value("project_id", std::vector<int>{}),
            i_player.value("project_levels", std::vector<int>{}),
            i_player.value("defeated_domains", std::vector<int>{}),
            std::move(temp_inventory),
            i_player.value("last_login_timestamp", 0LL)
        );

        players_list.push_back(std::move(temp_player));
    }
    return players_list;
}

void player::save_players(const std::vector<player>& players_list) {
    nlohmann::json json_players_list = nlohmann::json::array();

    for (const auto& p : players_list) {
        nlohmann::json p_json;
        p_json["name"] = p.username;
        p_json["password"] = p.password;
        p_json["currency1"] = p.currency1;
        p_json["currency2"] = p.currency2;
        p_json["health"] = p.health;
        p_json["damage"] = p.damage;
        p_json["conquer_domain"] = p.current_target_domain_id;
        p_json["last_login_timestamp"] = p.last_login_timestamp;
        p_json["project_id"] = p.project_id;
        p_json["project_levels"] = p.project_levels;
        p_json["defeated_domains"] = p.defeated_domains;
        p_json["inventory"] = nlohmann::json::array();

        for (const auto& item_ptr : p.inventory) {
            nlohmann::json item_json;
            item_json["type"] = item_ptr->get_type();
            item_json["name"] = item_ptr->get_name();
            item_json["description"] = item_ptr->get_description();
            item_json["price"] = item_ptr->get_price();
            item_json["rarity"] = item_ptr->get_rarity();
            item_json["consumable"] = item_ptr->get_consumable();

            if (item_ptr->get_type() == "book") {
                if (const auto b = dynamic_cast<const book*>(item_ptr.get())) {
                    item_json["damage_bonus"] = b->get_damage_bonus();
                    item_json["rarity"] = b->get_rarity();
                }
            }
            else if (item_ptr->get_type() == "drink") {
                if (const auto d = dynamic_cast<const drink*>(item_ptr.get())) {
                    item_json["health_restore"] = d->get_health_restore();
                }
            }
            else if (item_ptr->get_type() == "cheating_sheet") {
                if (const auto c = dynamic_cast<const cheating_sheet*>(item_ptr.get())) {
                    item_json["project_boost"] = c->get_project_boost();
                    item_json["success_chance"] = c->get_success_chance();
                    item_json["risk_damage"] = c->get_risk_damage();
                }
            }
            p_json["inventory"].push_back(item_json);
        }
        json_players_list.push_back(p_json);
    }

    std::ofstream f("players.json");
    f << json_players_list.dump(4);
    f.close();
}

bool player::verify_password() const {
    std::string temp_password;
    std::cout << "Required password>";
    std::getline(std::cin, temp_password);
    if (temp_password == this->get_password()) return true;
    return false;
}

int player::add_new_player(std::vector<player>& players_list) {
    player temp_player;
    std::cin >> temp_player;
    players_list.push_back(std::move(temp_player));
    return static_cast<int>(players_list.size() - 1);
}

void player::reset_projects() {
    project_id.clear();
    project_levels.clear();
}

void player::player_stats(std::ostream& os) const {
    os << "Player: " << this->username << "\n";
    os << "Currency1: " << this->currency1 << "\n";
    os << "Currency2: " << this->currency2 << "\n";
    os << "Health: " << this->health << "\n";
    os << "Damage: " << this->damage << "\n";
}

void player::player_full_stats(std::ostream& os, const std::vector<project>& projects_list) const {
    this->player_stats(os);
    os << "Projects list:\n";
    bool ok = false;
    for (size_t i = 0; i < this->project_levels.size() && i < projects_list.size(); ++i) {
        int level = this->project_levels[i];
        if (level > 0) {
            const project& p = projects_list[i];
            float current_income = p.get_cashback() * static_cast<float>(level);
            float next_upgrade_cost = p.get_price() * static_cast<float>(level + 1);
            os << "  - " << projects_list[i].get_name() << " [" << level << "]" << " Income: " << current_income << " Cashback: " << p.get_cashback() << " Next upgrade cost: " << next_upgrade_cost << "\n";
            ok = true;
        }
    }
    if (!ok) {
        os << "  No projects owned.\n";
    }
}

void player::show_stats() const {
    this->player_stats(std::cout);
    std::cout << "\n";
}

void player::show_projects_info(const std::vector<project> &projects_repo) const {
    std::cout << "Here you can see the projects you fought for! Upgrade them in order to increase you currencies and later you can even reset getting -RESTANTA- and grow stronger !\n";
    this->player_full_stats(std::cout, projects_repo);
    std::cout << "\n";
}

void player::reset_progress() {
    this->reset_game();
}

void player::enter_examination_room(const std::vector<teacher> &teachers_list) {
    std::cout << "You entered in Politehnica Business Tower! It's a strange place isn't it? They are ready!\n";

    std::vector<int> teacher_indices;
    for (int i = 0; i < static_cast<int>(teachers_list.size()); ++i) {
        teacher_indices.push_back(i);
    }

    std::random_device rd;
    std::mt19937 generator(rd());
    int available_rerolls = 3;

    while (available_rerolls >= 0) {
        std::ranges::shuffle(teacher_indices, generator);

        std::cout << "You faced:\n";
        for (int i = 0; i < 5 && i < static_cast<int>(teacher_indices.size()); ++i) {
            int current_teacher_id = teacher_indices[i];
            std::cout << i << ". " << teachers_list[current_teacher_id] << "\n";
        }

        if (available_rerolls > 0) {
            int ok;
            std::cout << "Reroll (1) or Keep (0)? (" << available_rerolls << " left)\n";
            std::cin >> ok;
            if (ok == 0) {
                std::cout << "Selection kept. Proceeding...\n";
                break;
            }
            if (ok == 1) available_rerolls--;
            else break;
        } else {
            std::cout << "No rerolls left.\n";
            break;
        }
    }

    int recommended_slot = 0;
    float min_hp = std::numeric_limits<float>::max();

    for(int i = 0; i < 5; ++i) {
        int real_id = teacher_indices[i];
        float current_hp = teachers_list[real_id].get_health();

        if(current_hp < min_hp) {
            min_hp = current_hp;
            recommended_slot = i;
        }
    }
    std::cout << "\n Hint: Weakest teacher is at option " << recommended_slot << " with " << min_hp << " hp.\n";

    int fight_t = -1;
    int aux_t_index = -1;
    do {
        std::cout << "Who do you wanna fight? (0-4; 5 for auto-pick the -weakest- one)\n";
        std::cin >> fight_t;
        if (fight_t == 5) {
            std::cout << "Auto-picking option " << recommended_slot << "...\n";
            fight_t = recommended_slot;
        }
        if (fight_t >= 0 && fight_t < 5 && fight_t < static_cast<int>(teacher_indices.size())) {
            aux_t_index = teacher_indices[fight_t];
        } else {
            std::cout << "Invalid selection.\n";
        }
    } while (aux_t_index == -1);

    const teacher &teacher_fought = teachers_list[aux_t_index];
    this->fight_teacher(teacher_fought, aux_t_index);
}

void player::perform_upgrade(const std::vector<project> &projects_list) {
    const std::vector<int> &my_levels = this->get_project_id();

    std::cout << "Buy projects : \n";
    bool ok = false;

    for (size_t i = 0; i < my_levels.size(); ++i) {
        int level = my_levels[i];
        if (level > 0 && i < projects_list.size()) {
            const project &p = projects_list[i];
            float cost = p.get_price() * static_cast<float>(level + 1);
            std::cout << i << ". " << p.get_name() << " lvl: " << level
                      << " cost: " << cost << " cashback: " << p.get_cashback() << "\n";
            ok = true;
        }
    }

    if (!ok) {
        std::cout << "Fight with a teacher first!\n";
        return;
    }

    int selected_id;
    std::cout << "\nWhich project will you upgrade?? ";
    std::cin >> selected_id;

    if (selected_id < 0 || selected_id >= static_cast<int>(projects_list.size()) ||
        selected_id >= static_cast<int>(my_levels.size()) || my_levels[selected_id] == 0) {
        std::cout << "Invalid or unowned project id! \n";
        return;
        }

    this->project_upgrade(selected_id, projects_list);
}


void player::add_project_id(int id) {
    if (id >= static_cast<int>(project_id.size())) {
        project_id.resize(id + 1, 0);
        project_levels.resize(id + 1, 0);
    }
    project_id[id]++;
    project_levels[id]++;
}

void player::add_item(std::unique_ptr<item> new_item) {
    this->inventory.push_back(std::move(new_item));
    std::cout << "Item added to your inventory!\n";
}

void player::show_inventory() const {
    if (inventory.empty()) {
        std::cout << "Your inventory is empty.\n";
        return;
    }
    std::cout << username << "'s Inventory \n";
    for (size_t i = 0; i < inventory.size(); ++i) {
        std::cout << i + 1 << ". [" << inventory[i]->get_type() << "] " << inventory[i]->get_name() << "\n";
        std::cout << "   Description: " << inventory[i]->get_description() << "\n";
    }
}

void player::add_defeated_domain(int domain_id) {
    defeated_domains.push_back(domain_id);
}

void player::calculate_and_set_conquer_domain() {
    if (defeated_domains.empty()) {
        this->current_target_domain_id = -1;
        return;
    }
    std::ranges::sort(defeated_domains);
    int max_count = 0;
    int mfdomain = -1;
    int curr_d = defeated_domains[0];
    int curr_c = 0;
    for (int domain : defeated_domains) {
        if (domain == curr_d) {
            curr_c++;
        } else {
            if (curr_c > max_count) {
                max_count = curr_c;
                mfdomain = curr_d;
            }
            curr_d = domain;
            curr_c = 1;
        }
    }
    if (curr_c > max_count) {
        mfdomain = curr_d;
    }
    this->current_target_domain_id = mfdomain;
}

int player::turns_to_defeat(float enemy_hp) const {
    if (this->damage <= 0.0f) {
        return (enemy_hp > 0) ? -1 : 0;
    }
    float average_damage = damage + 0.2f * this->health;
    float turns_float = enemy_hp / average_damage;
    return static_cast<int>(turns_float);
}

void player::project_upgrade(int selected_id, const std::vector<project>& projects_list) {

    if (selected_id < 0 || selected_id >= static_cast<int>(projects_list.size())) {
        std::cout << "Invalid project ID for upgrade!\n";
        return;
    }

    const project& selected_project = projects_list[selected_id];
    const std::vector<int>& p_project_levels = this->get_project_id();

    if (selected_id >= static_cast<int>(p_project_levels.size()) || p_project_levels[selected_id] == 0) {
        std::cout << "You don't own project " << selected_id << ". This should be checked in menu!\n";
        return;
    }

    int current_level = p_project_levels[selected_id];
    float cost = selected_project.get_price() * static_cast<float>(current_level + 1);
    float current_currency = this->get_currency1();

    if (current_currency >= cost) {
        this->set_currency1(current_currency - cost);
        this->add_project_id(selected_id);

        float reward = selected_project.get_cashback() * static_cast<float>(current_level + 1) * 10.0f;
        this->set_currency1(this->get_currency1() + reward);

        std::cout << "\n[SUCCES] project \"" << selected_project.get_name() << "\" was upgraded to level " << current_level + 1 << "\n";
        std::cout << "New currency1: " << this->get_currency1() << "\n";
    } else {
        std::cout << "Not enough money! Required: " << cost << ", Available: " << current_currency << ".\n";
    }
}

void player::fight_teacher(const teacher& opponent, int p_project_id) {
    float critical_chance = opponent.get_critical_chance();

    int player_turns = this->turns_to_defeat(opponent.get_health());
    int teacher_turns = opponent.turns_to_defeat(this->get_health(), critical_chance);

    if (player_turns != -1 && (teacher_turns == -1 || player_turns <= teacher_turns)) {
        std::cout << opponent.get_last_name() << " defeated! Well done! You got project no." << p_project_id << "\n";

        this->add_project_id(p_project_id);
        int domain_id = opponent.get_domain();
        this->add_defeated_domain(domain_id);
        this->calculate_and_set_conquer_domain();
    } else {
        std::cout << "You died! Teacher " << opponent.get_last_name() << " was stronger." << "\n";
    }
}

void player::idle_earnings(const std::vector<project>& projects_list) {
    long long current_time = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    long long time_elapsed_seconds = current_time - get_last_login_timestamp();
    constexpr long long parse_seconds = 5 * 60;

    if (time_elapsed_seconds <= 0) {
        set_last_login_timestamp(current_time);
        return;
    }

    long long tick_count = time_elapsed_seconds / parse_seconds;

    if (tick_count > 0) {
        float total_idle_earnings = 0.0f;
        const std::vector<int> &i_project_levels = get_project_levels();

        for (size_t i = 0; i < i_project_levels.size() && i < projects_list.size(); ++i) {
            int level = i_project_levels[i];
            if (level > 0) {
                const project &p = projects_list[i];
                float earnings_per_tick = p.get_cashback() * static_cast<float>(level);
                total_idle_earnings += earnings_per_tick;
            }
        }

        float final_earnings = total_idle_earnings * static_cast<float>(tick_count);
        set_currency1(get_currency1() + final_earnings);

        std::cout << "\nIdle Income: You were away for " << time_elapsed_seconds / 3600 << " hours and " <<(time_elapsed_seconds % 3600) / 60 << " minutes and " << (time_elapsed_seconds % 60) << " seconds.\n";
        std::cout << "You earned " << final_earnings << " currency1!\n";
    }
    set_last_login_timestamp(current_time);
}

void player::reset_game() {
    float c1_to_reset = get_currency1();

    float multiplier = 5.0f;
    float earned_currency2 = std::log10(c1_to_reset + 1.0f) * multiplier;

    std::cout << "Do you want to reset? \n";
    std::cout << "You have " << c1_to_reset << " currency1 and will earn " << earned_currency2 << " currency2.\n";
    std::cout << "This will reset your Currency1 and all Project levels.\n";
    std::cout << "Type 'ok' to confirm the reset or anything else not to: ";

    std::string confirmation;
    std::cin >> confirmation;

    if (confirmation == "ok") {
        set_currency2(get_currency2() + earned_currency2);
        set_currency1(0.0f);
        this->reset_projects();
        std::cout << "Success!\n";
    } else {
        std::cout << "Cancelled.\n";
    }
}

inline std::istream& operator>>(std::istream& is, player& t) {
    std::string temp_pass1, temp_pass2;
    t.currency1 = 100.0;
    t.currency2 = 0.0;
    t.current_target_domain_id = -1;
    t.health = 500.0;
    t.damage = 7.0;
    t.project_id.clear();
    std::cout << "NEW PLAYER REGISTRATION\n";
    std::cout << "Insert player name: ";
    getline(is, t.username);
    do {
        std::cout << "Password must be at least 5 chars long!\n";
        std::cout << "Insert your password> ";
        is >> temp_pass1;
        std:: cout << "Re-type your password# ";
        is >> temp_pass2;
        if (temp_pass1 != temp_pass2) std::cout << "Retry. Passwords doesn't match!\n";
        if (temp_pass1.size() < 5) std::cout << "Password must be at least 5 chars long!\n";
    } while (temp_pass1 != temp_pass2 || temp_pass1.size()< 5);
    t.password = temp_pass1;
    return is;
}

std::ostream& operator<<(std::ostream& os, const player& t) {
    os << t.username;
    return os;
}

