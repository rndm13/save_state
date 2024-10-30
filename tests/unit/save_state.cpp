#include "../../src/save_state.hpp"
#include "../save_state_test_data.hpp"
#include "gtest/gtest.h"

TEST(save_state, save) {
    TestData input = {};

    SaveState ss = {};
    ss.save(input);
    ss.finish_save();
}

TEST(save_state, load) {
    TestData input = {};

    SaveState ss = {};
    ss.save(input);
    ss.finish_save();
    ss.reset_load();

    TestData got = {
        .id = "uuid",
        .points = {},
        .average_points = std::nullopt,
        .dictionary = {},
    };
    ASSERT_TRUE(ss.load(got));
    EXPECT_EQ(ss.load_idx, ss.original_size);
    EXPECT_EQ(input, got);
}

TEST(save_state, load_alt_data) {
    TestData input = {
        .id = "uuid",
        .points = {},
        .average_points = std::nullopt,
        .dictionary = {},
    };

    SaveState ss = {};
    ss.save(input);
    ss.finish_save();
    ss.reset_load();

    TestData got = {};

    ASSERT_TRUE(ss.load(got));
    EXPECT_EQ(ss.load_idx, ss.original_size);
    EXPECT_EQ(input, got);
}
