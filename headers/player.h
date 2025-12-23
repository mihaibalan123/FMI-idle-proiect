#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <utility>
#include <memory>
#include <algorithm>
#include "project.h"
#include "teacher.h"
#include "item.h"
#include <vector>
#include <chrono>

class player {
    std::string username, password;
    int current_target_domain_id;
    float currency1, currency2, health, damage;
    std::vector<int> project_id;
    std::vector<int> project_levels;
    std::vector<int> defeated_domains;
    std::vector<std::unique_ptr<item> > inventory;
    long long last_login_timestamp = 0;

    void add_project_id(int id);

    void add_defeated_domain(int domain_id);

    void calculate_and_set_conquer_domain();

    void player_stats(std::ostream &os) const;

    void reset_projects();

    void player_full_stats(std::ostream &os, const std::vector<project> &projects_list) const;

    void reset_game();

    void fight_teacher(const teacher &opponent, int p_project_id);

    void project_upgrade(int selected_id, const std::vector<project> &projects_list);

public:
    player(std::string name, std::string password, int conquer_domain, float currency1, float currency2, float health,
           float damage, std::vector<int> project_id, std::vector<int> project_levels,
           std::vector<int> defeated_domains = {}, std::vector<std::unique_ptr<item>> inventory = {}, long long last_login_timestamp = 0) : username(std::move(name)),
        password(std::move(password)), current_target_domain_id(conquer_domain), currency1(currency1),
        currency2(currency2), health(health), damage(damage), project_id(std::move(project_id)),
        project_levels(std::move(project_levels)), defeated_domains(std::move(defeated_domains)),inventory(std::move(inventory)),
        last_login_timestamp(last_login_timestamp)  {
    };

    player() : current_target_domain_id(0), currency1(0.0f), currency2(0.0f), health(0.0f), damage(0.0f),
               last_login_timestamp(
                   std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch())
                   .count()) {
    }

    ~player() = default;

    player(const player& other)
        : username(other.username),
          password(other.password),
          current_target_domain_id(other.current_target_domain_id),
          currency1(other.currency1),
          currency2(other.currency2),
          health(other.health),
          damage(other.damage),
          project_id(other.project_id),
          project_levels(other.project_levels),
          defeated_domains(other.defeated_domains),
          last_login_timestamp(other.last_login_timestamp)
    {
        for (const auto& item_ptr : other.inventory) {
            this->inventory.push_back(std::unique_ptr<item>(item_ptr->clone()));
        }
    }

    friend void swap(player& first, player& second) noexcept {
        using std::swap;
        swap(first.username, second.username);
        swap(first.password, second.password);
        swap(first.inventory, second.inventory);
        swap(first.project_levels, second.project_levels);
        swap(first.project_id, second.project_id);
        swap(first.defeated_domains, second.defeated_domains);
        swap(first.health, second.health);
        swap(first.currency1, second.currency1);
        swap(first.currency2, second.currency2);
        swap(first.damage, second.damage);
        swap(first.current_target_domain_id, second.current_target_domain_id);
        swap(first.last_login_timestamp, second.last_login_timestamp);
    }

    player& operator=(player other) {
        swap(*this, other);
        return *this;
    }


    void add_item(std::unique_ptr<item> new_item);

    void show_inventory() const;

    void show_stats() const;

    void show_projects_info(const std::vector<project> &projects_repo) const;

    void reset_progress();

    void enter_examination_room(const std::vector<teacher> &teachers_list);

    void perform_upgrade(const std::vector<project> &projects_list);

    [[nodiscard]] const std::string &get_password() const;

    [[nodiscard]] const std::vector<int> &get_project_id() const;

    [[nodiscard]] float get_health() const;

    [[nodiscard]] float get_damage() const;

    [[nodiscard]] float get_currency1() const;

    [[nodiscard]] const std::vector<int> &get_project_levels() const;

    [[nodiscard]] float get_currency2() const;

    [[nodiscard]] long long get_last_login_timestamp() const;

    [[nodiscard]] size_t get_inventory_size() const;

    [[nodiscard]] item* get_item_at(size_t index) const;

    void remove_item_at(size_t index);

    void set_currency1(float value);

    void set_currency2(float value);

    void set_health(float value);

    void set_damage(float value);

    void set_last_login_timestamp(long long timestamp);

    static std::vector<player> load_players();

    static void save_players(const std::vector<player> &players_list);

    static int add_new_player(std::vector<player> &players_list);

    [[nodiscard]] bool verify_password() const;

    void idle_earnings(const std::vector<project> &projects_list);

    [[nodiscard]] float get_total_damage() const;

    void use_items_menu();

    [[nodiscard]] bool has_any_project() const;

    bool boost_random_project(int value);

    void respawn();

    void start_easy_job();

    void start_complex_job();

    [[nodiscard]] bool has_item(const std::string& item_name) const;

    friend std::istream &operator>>(std::istream &is, player &t);

    friend std::ostream &operator<<(std::ostream &os, const player &t);

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(player, username, current_target_domain_id, currency1, currency2, password, health,
                                   damage, project_id, project_levels, defeated_domains, last_login_timestamp)
};

#endif //OOP_PLAYER_H
