#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <utility>
#include <algorithm>
#include "project.h"
#include "teacher.h"
#include <vector>
#include <chrono>

class player {
    std::string username, password;
    int current_target_domain_id;
    float currency1, currency2, health, damage;
    std::vector<int> project_id;
    std::vector<int> project_levels;
    std::vector<int> defeated_domains;
    long long last_login_timestamp = 0;
    void add_project_id(int id);
    void add_defeated_domain(int domain_id);
    void calculate_and_set_conquer_domain();
    void player_stats(std::ostream& os) const;
    void reset_projects();
    void player_full_stats(std::ostream& os, const std::vector<project>& projects_list) const;
    void reset_game();
    void fight_teacher(const teacher& opponent, int p_project_id);
    void project_upgrade(int selected_id, const std::vector<project>& projects_list);
    [[nodiscard]] int turns_to_defeat(float enemy_hp) const;
public:
    player(std::string name, std::string password, int conquer_domain, float currency1, float currency2, float health, float damage, std::vector<int> project_id,std::vector<int> project_levels, std::vector<int> defeated_domains = {}, long long last_login_timestamp  = 0): username(std::move(name)),password(std::move(password)), current_target_domain_id(conquer_domain), currency1(currency1), currency2(currency2), health(health), damage(damage), project_id(std::move(project_id)),project_levels(std::move(project_levels)), defeated_domains(std::move(defeated_domains)), last_login_timestamp(last_login_timestamp) {};
    player() : current_target_domain_id(0), currency1(0.0f), currency2(0.0f), health(0.0f), damage(0.0f), last_login_timestamp(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()) {}
    ~player() = default;

    void show_stats() const;
    void show_projects_info(const std::vector<project>& projects_repo) const;
    void reset_progress();
    void enter_examination_room(const std::vector<teacher>& teachers_list);
    void perform_upgrade(const std::vector<project>& projects_list);

    [[nodiscard]] const std::string& get_password() const;
    [[nodiscard]] const std::vector<int>& get_project_id() const;
    [[nodiscard]] float get_health() const;
    [[nodiscard]] float get_currency1() const;
    [[nodiscard]] int get_conquer_domain() const;
    [[nodiscard]] const std::string& get_name() const;
    [[nodiscard]] float get_damage() const;
    [[nodiscard]] const std::vector<int>& get_defeated_domains() const;
    [[nodiscard]] const std::vector<int>& get_project_levels() const;
    [[nodiscard]] float get_currency2() const;
    [[nodiscard]] long long get_last_login_timestamp() const;

    void set_currency1(float value);
    void set_currency2(float value);
    void set_last_login_timestamp(long long timestamp);

    static std::vector<player> load_players();
    static void save_players(const std::vector<player>& players_list);
    static int add_new_player(std::vector<player>& players_list);
    [[nodiscard]] bool verify_password() const;
    void idle_earnings(const std::vector<project>& projects_list);

    friend std::istream& operator>>(std::istream& is, player &t);
    friend std::ostream& operator<<(std::ostream& os, const player& t);
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(player, username, current_target_domain_id, currency1, currency2, password, health, damage, project_id, project_levels, defeated_domains, last_login_timestamp)
};

#endif //OOP_PLAYER_H