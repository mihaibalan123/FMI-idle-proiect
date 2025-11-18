#include "teacher.h"

std::vector<teacher> teacher::load_teachers(const std::vector<project>& projects_list) {

    std::vector<teacher> teachers_list;
    std::ifstream f("teachers.json");
    nlohmann::json data = nlohmann::json::parse(f);

    for (const auto &i_teacher: data) {
        int project_idx = i_teacher["assigned_project"];
        if (project_idx >= 0 && project_idx < static_cast<int>(projects_list.size())) {
            const project &assigned_project = projects_list[project_idx];

            teacher temp_teacher(
                i_teacher["last_name"],
                i_teacher["first_name"],
                i_teacher["rarity"],
                i_teacher["domain"],
                i_teacher["aura"],
                i_teacher["item_id"],
                i_teacher["health"],
                i_teacher["damage"],
                i_teacher["critical_damage"],
                assigned_project
            );
            teachers_list.push_back(temp_teacher);
        } else {
            teacher temp_teacher(
                i_teacher["last_name"],
                i_teacher["first_name"],
                i_teacher["rarity"],
                i_teacher["domain"],
                i_teacher["aura"],
                i_teacher["item_id"],
                i_teacher["health"],
                i_teacher["damage"],
                i_teacher["critical_damage"],
                project()
            );
            teachers_list.push_back(temp_teacher);
        }
    }
    return teachers_list;
}

int teacher::turns_to_defeat(float enemy_hp, float t_critical_chance) const {
    if (this->damage <= 0.0f) {
        return (enemy_hp > 0) ? -1 : 0;
    }
    float average_damage = this->damage * (1.0f + this->critical_chance * t_critical_chance);
    float turns_float = enemy_hp / average_damage;
    return static_cast<int>(turns_float);
}

