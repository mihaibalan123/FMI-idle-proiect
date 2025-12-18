#include "player.h"
#include "teacher.h"
#include "book.h"
#include "cheating_sheet.h"
#include "drink.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

const std::string &player::get_password() const {
    return password;
}

const std::vector<int> &player::get_project_id() const {
    return project_id;
}

const std::vector<int> &player::get_project_levels() const {
    return project_levels;
}

float player::get_health() const {
    return health;
}

float player::get_damage() const {
    return damage;
}

float player::get_currency1() const {
    return currency1;
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

void player::set_damage(float value) {
    damage = value;
}

void player::set_last_login_timestamp(long long timestamp) {
    last_login_timestamp = timestamp;
}

std::vector<player> player::load_players() {
    std::vector<player> players_list;
    std::ifstream f2("players.json");
    nlohmann::json data2 = nlohmann::json::parse(f2);

    for (auto &i_player: data2) {
        std::vector<std::unique_ptr<item> > temp_inventory;
        if (i_player.contains("inventory")) {
            for (const auto &item_json: i_player["inventory"]) {
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
                } else if (type == "drink") {
                    float hp = item_json.value("health_restore", 0.0f);
                    temp_inventory.push_back(std::make_unique<drink>(
                        i_name, i_desc, i_rarity, i_price, i_consumable, hp
                    ));
                } else if (type == "cheating_sheet") {
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

void player::save_players(const std::vector<player> &players_list) {
    nlohmann::json json_players_list = nlohmann::json::array();

    for (const auto &p: players_list) {
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

        for (const auto &item_ptr: p.inventory) {
            nlohmann::json item_json;
            item_json["type"] = item_ptr->get_type();
            item_json["name"] = item_ptr->get_name();
            item_json["description"] = item_ptr->get_description();
            item_json["price"] = item_ptr->get_price();
            item_json["rarity"] = item_ptr->get_rarity();
            item_json["consumable"] = item_ptr->get_consumable();

            if (item_ptr->get_type() == "book") {
                if (const auto b = dynamic_cast<const book *>(item_ptr.get())) {
                    item_json["damage_bonus"] = b->get_damage_bonus();
                    item_json["rarity"] = b->get_rarity();
                }
            } else if (item_ptr->get_type() == "drink") {
                if (const auto d = dynamic_cast<const drink *>(item_ptr.get())) {
                    item_json["health_restore"] = d->get_health_restore();
                }
            } else if (item_ptr->get_type() == "cheating_sheet") {
                if (const auto c = dynamic_cast<const cheating_sheet *>(item_ptr.get())) {
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

int player::add_new_player(std::vector<player> &players_list) {
    player temp_player;
    std::cin >> temp_player;
    players_list.push_back(std::move(temp_player));
    return static_cast<int>(players_list.size() - 1);
}

void player::reset_projects() {
    project_id.clear();
    project_levels.clear();
}

void player::player_stats(std::ostream &os) const {
    os << "Player: " << this->username << "\n";
    os << "Currency1: " << this->currency1 << "\n";
    os << "Currency2: " << this->currency2 << "\n";
    os << "Health: " << this->health << "\n";
    os << "Damage: " << this->damage << "\n";
}

void player::player_full_stats(std::ostream &os, const std::vector<project> &projects_list) const {
    this->player_stats(os);
    os << "Projects list:\n";
    bool ok = false;
    for (size_t i = 0; i < this->project_levels.size() && i < projects_list.size(); ++i) {
        int level = this->project_levels[i];
        if (level > 0) {
            const project &p = projects_list[i];
            float current_income = p.get_cashback() * static_cast<float>(level);
            float next_upgrade_cost = p.get_price() * static_cast<float>(level + 1);
            os << "  - " << projects_list[i].get_name() << " [" << level << "]" << " Income: " << current_income <<
                    " Cashback: " << p.get_cashback() << " Next upgrade cost: " << next_upgrade_cost << "\n";
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
    std::cout <<
            "Here you can see the projects you fought for! Upgrade them in order to increase you currencies and later you can even reset getting -RESTANTA- and grow stronger !\n";
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

    for (int i = 0; i < 5; ++i) {
        int real_id = teacher_indices[i];
        float current_hp = teachers_list[real_id].get_health();

        if (current_hp < min_hp) {
            min_hp = current_hp;
            recommended_slot = i;
        }
    }
    std::cout << "\n Hint: Weakest teacher is at option " << recommended_slot << " with " << min_hp << " hp.\n";

    int fight_t = -1;
    int aux_t_index = -1;
    do {
        std::cout << "Who do you wanna fight? (0-4; 5 for auto-pick the -weakest- one)\n";
        std::cout << ">";
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
    this->use_items_menu();
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
        std::cout << *inventory[i] << "\n";
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
    for (int domain: defeated_domains) {
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

void player::project_upgrade(int selected_id, const std::vector<project> &projects_list) {
    if (selected_id < 0 || selected_id >= static_cast<int>(projects_list.size())) {
        std::cout << "Invalid project ID for upgrade!\n";
        return;
    }

    const project &selected_project = projects_list[selected_id];
    const std::vector<int> &p_project_levels = this->get_project_id();

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

        std::cout << "\n[SUCCES] project \"" << selected_project.get_name() << "\" was upgraded to level " <<
                current_level + 1 << "\n";
        std::cout << "New currency1: " << this->get_currency1() << "\n";
    } else {
        std::cout << "Not enough money! Required: " << cost << ", Available: " << current_currency << ".\n";
    }
}

void player::fight_teacher(const teacher &opponent, int p_project_id) {
    float player_damage = this->get_total_damage();
    float teacher_damage = 10.0f + (opponent.get_health() * 0.1f);
    float teacher_hp = opponent.get_health();

    std::cout << "\n You faced " << opponent.get_last_name() << "\n";
    std::cout << "You (hp: " << this->health << ", damage: " << player_damage << ") vs Teacher (hp: " << teacher_hp <<
            ", damage: " << teacher_damage << ")\n";

    int turns_to_win = (player_damage > 0) ? static_cast<int>(std::ceil(teacher_hp / player_damage)) : 999;

    int turns_to_die = (teacher_damage > 0) ? static_cast<int>(std::ceil(this->health / teacher_damage)) : 999;

    if (turns_to_win <= turns_to_die) {
        std::cout << "Victory! You defeated " << opponent.get_last_name() << "!\n";

        float damage_taken = static_cast<float>(turns_to_win) * teacher_damage;

        this->health -= damage_taken;
        if (this->health < 0) this->health = 0;

        std::cout << "You took " << damage_taken << " damage during the fight.\n";
        std::cout << "Remaining Health: " << this->health << "\n";

        std::cout << "You got project no." << p_project_id << "\n \n";
        this->add_project_id(p_project_id);
        int domain_id = opponent.get_domain();
        this->add_defeated_domain(domain_id);
        this->calculate_and_set_conquer_domain();
    } else {
        this->health = 0;
        std::cout << "Defeated! " << opponent.get_last_name() << " was too strong.\n";
        std::cout << "You needed " << turns_to_win << " turns, but could only survive " << turns_to_die << ".\n";
        this->respawn();
    }
}

void player::idle_earnings(const std::vector<project> &projects_list) {
    long long current_time = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    long long time_elapsed_seconds = current_time - get_last_login_timestamp();
    constexpr long long parse_seconds = 5 * 60;

    set_last_login_timestamp(current_time);

    if (time_elapsed_seconds <= 0) {
        return;
    }

    std::cout << "\nIdle Income: You were away for " << time_elapsed_seconds / 3600 << " hours and " << (
            time_elapsed_seconds % 3600) / 60 << " minutes and " << (time_elapsed_seconds % 60) << " seconds.\n";

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
        std::cout << "You earned " << final_earnings << " currency1!\n";
    }

    float max_hp = 500.0f;

    if (this->health < max_hp) {
        long long seconds_per_hp = 150;
        float hp_regen_rate = 1.0f;

        long long ticks = time_elapsed_seconds / seconds_per_hp;
        float hp_gained = static_cast<float>(ticks) * hp_regen_rate;

        if (hp_gained > 0) {
            float old_hp = this->health;
            this->health += hp_gained;

            if (this->health > max_hp) {
                this->health = max_hp;
            }

            std::cout << "You recovered " << (this->health - old_hp) << " hp while resting.\n";
            std::cout << "Current Health: " << this->health << " / " << max_hp << "\n";
            std::cout << "\n";
        }
    } else {
        std::cout << " Current Health: maximum (fully rested).\n";
        std::cout << "\n";
    }

}

bool player::has_any_project() const {
    return std::ranges::any_of(project_levels, [](int i) {
        return i > 0;
    });
}

bool player::boost_random_project(int value) {
    std::vector<int> projects;
    for (size_t i = 0; i < project_levels.size(); ++i) {
        if (project_levels[i] > 0) projects.push_back(static_cast<int>(i));
    }

    if (projects.empty()) return false;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, static_cast<int>(projects.size()) - 1);

    int random_idx = projects[distr(gen)];
    project_levels[random_idx] += value;

    std::cout << "Project ID " << random_idx << " increased by " << value << " levels!\n";
    return true;
}

float player::get_total_damage() const {
    float total = this->damage;
    for (const auto &i: inventory) {
        if (i->get_type() == "book") {
            if (!i->get_consumable()) {
                if (const auto b = dynamic_cast<const book *>(i.get())) {
                    total += b->get_damage_bonus();
                }
            }
        }
    }
    return total;
}

void player::use_items_menu() {
    if (inventory.empty()) return;
    std::cout << "\n Player's inventory \n";
    for (size_t i = 0; i < inventory.size(); ++i) {
        std::cout << i + 1 << ". " << inventory[i]->get_name()
                << " [" << inventory[i]->get_type() << "]"
                << (inventory[i]->get_consumable() ? " (Consumable)" : " (Passive)")
                << "\n";
    }

    int idx;
    std::cout << "Select item ID to use (0 to skip): ";
    std::cin >> idx;

    if (idx > 0 && idx <= static_cast<int>(inventory.size())) {
        auto &item_ptr = inventory[idx - 1];
        item_ptr->use(*this);
        if (item_ptr->get_consumable()) {
            if (item_ptr->get_type() == "cheating_sheet" && !this->has_any_project()) {
                std::cout << "(Item kept in inventory because it had no effect)\n";
            } else {
                inventory.erase(inventory.begin() + (idx - 1));
                std::cout << "-> Item removed from inventory.\n";
            }
        }
    }
}

void player::respawn() {
    std::cout << "\n=== YOU FAINTED ===\n";
    std::cout << "You were carried out of the examination room to the nurse's office.\n";

    this->health = 1.0f;

    std::cout << "-> Health set at 1 hp! \n";

    float penalty = 0.50f;
    float lost_money = this->currency1 * penalty;

    this->currency1 -= lost_money;
    if (this->currency1 < 0) this->currency1 = 0;

    std::cout << "-> You paid " << lost_money << " currency1 for the medical bill.\n";
    std::cout << "-> Remaining currency1: " << this->currency1 << "\n";

    std::cout << "Don't give up! Come back when you are stronger!\n";
}

void player::start_easy_job() {
    std::cout << "\n You've got easiest job of your life! Start doing your job... \n";
    std::cout << "Type '-999' to quit.\n\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> num_dist(1, 50);
    std::uniform_int_distribution<> op_dist(0, 2);

    while (true) {
        int a = num_dist(gen);
        int b = num_dist(gen);
        int op = op_dist(gen);

        long long correct_answer = 0;
        char op_char;

        if (op == 2) {
            a = (a % 12) + 1;
            b = (b % 12) + 1;
            op_char = '*';
            correct_answer = static_cast<long long>(a) * b;
        } else if (op == 1) {
            if (a < b) std::swap(a, b);
            op_char = '-';
            correct_answer = a - b;
        } else {
            op_char = '+';
            correct_answer = static_cast<long long>(a) + b;
        }

        std::cout << a << " " << op_char << " " << b << " = ";

        long long user_answer;
        while (!(std::cin >> user_answer)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Numbers only! Try again: ";
        }

        if (user_answer == -999) break;

        if (user_answer == correct_answer) {
            float reward = 35.0f;
            this->currency1 += reward;
            std::cout << " -> Correct! + " << reward << " currency1.\n";
        } else {
            std::cout << " -> Wrong! Answer: " << correct_answer << ".\n";
        }
    }
}

static long long calculate_op(long long a, long long b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        default: return 0;
    }
}

void player::start_complex_job() {
    std::cout << "Complex expressions, higher money. Watch out!\n";
    std::cout << "Type '-999' to quit.\n\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> small_num(2, 10);
    std::uniform_int_distribution<> medium_num(10, 50);
    std::uniform_int_distribution<> structure_dist(0, 1);
    std::uniform_int_distribution<> op_dist(0, 2);
    std::string ops = "+-*";

    while (true) {
        char op1 = ops[op_dist(gen)];
        char op2 = ops[op_dist(gen)];

        long long a, b, c;

        if (op1 == '*') {
            a = small_num(gen);
        } else {
            a = medium_num(gen);
        }

        if (op1 == '*' || op2 == '*') {
            b = small_num(gen);
        } else {
            b = medium_num(gen);
        }

        if (op2 == '*') {
            c = small_num(gen);
        } else {
            c = medium_num(gen);
        }

        long long correct_answer = 0;
        std::string expression;

        int structure = structure_dist(gen);

        if (structure == 0) {
            long long step1 = calculate_op(a, b, op1);
            correct_answer = calculate_op(step1, c, op2);
            expression = "(" + std::to_string(a) + " " + op1 + " " + std::to_string(b) + ") "
                                + op2 + " " + std::to_string(c);
        } else {
            long long step1 = calculate_op(b, c, op2);
            correct_answer = calculate_op(a, step1, op1);
            expression = std::to_string(a) + " " + op1 + " (" + std::to_string(b) + " "
                                + op2 + " " + std::to_string(c) + ")";
        }

        std::cout <<  expression << " = ";

        long long answer;
        while (!(std::cin >> answer)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Numbers only! Try again: ";
        }

        if (answer == -999) break;

        if (answer == correct_answer) {
            float reward = 55.0f;
            this->currency1 += reward;
            std::cout << " -> Genius! + " << reward << " currency1.\n";
        } else {
            std::cout << " -> Wrong! Answer: " << correct_answer << ".\n";
        }
    }
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

inline std::istream &operator>>(std::istream &is, player &t) {
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
        std::cout << "Re-type your password# ";
        is >> temp_pass2;
        if (temp_pass1 != temp_pass2) std::cout << "Retry. Passwords doesn't match!\n";
        if (temp_pass1.size() < 5) std::cout << "Password must be at least 5 chars long!\n";
    } while (temp_pass1 != temp_pass2 || temp_pass1.size() < 5);
    t.password = temp_pass1;
    return is;
}

std::ostream &operator<<(std::ostream &os, const player &t) {
    os << t.username;
    return os;
}