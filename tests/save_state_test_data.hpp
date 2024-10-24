#pragma once

#include "../src/save_state.hpp"

struct TestData {
    std::variant<int32_t, std::string> id = 103;
    std::string name = "Sasha";
    std::string password = "12345678";
    std::vector<int64_t> points = {0, 2, 3, 5};
    std::optional<int64_t> average_points = 3;
    std::unordered_map<int32_t, std::string> dictionary = {{1, "one"}, {2, "two"}, {3, "three"}};

    SAVE_STATE_SAVE_IMPL(
            id, 0,
            name, 0,
            password, 0,
            points, 0,
            average_points, 0,
            dictionary, 0);
};

inline bool operator==(const TestData& first, const TestData& second) {
    return first.id == second.id && first.name == second.name &&
           first.password == second.password && first.points == second.points &&
           first.dictionary == second.dictionary;
}
