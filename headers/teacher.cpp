#include "teacher.h"
#include "exception.h"

const std::string &teacher::get_last_name() const {
    return last_name;
}

float teacher::get_health() const {
    return health;
}

int teacher::get_domain() const {
    return domain;
}

std::vector<teacher> teacher::load_teachers(const std::vector<project> &projects_list) {
    std::vector<teacher> teachers_list;
    nlohmann::json data = load_json_verified("teachers.json");

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
