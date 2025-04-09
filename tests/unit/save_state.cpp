#include "../../src/save_state.hpp"
#include "../save_state_test_data.hpp"
#include "gtest/gtest.h"
#include <fstream>
#include <cstdio>

TEST(save_state, save) {
    TestData input = {};

    SaveState ss = {};
    ss.save(input);
    ss.finish_save();
}

TEST(save_state, betoh_htobe) {
    char input[] = "abcd";
    details::htobe<4>(input);

    static constexpr char hbe[] = "abcd"; // No change
    static constexpr char hle[] = "dcba"; // Reverse order
    if constexpr (std::endian::native == std::endian::little) {
        EXPECT_STREQ(input, hle);
    } else {
        EXPECT_STREQ(input, hbe);
    }
}

TEST(save_state, load_be16) {
    uint16_t input = 1234;

    SaveState ss = {};
    ss.save(input);
    ss.finish_save();
    ss.reset_load();

    uint16_t output = 0;

    ASSERT_TRUE(ss.load(output));
    EXPECT_EQ(input, output);
}

TEST(save_state, load_be32) {
    uint32_t input = 12345678;

    SaveState ss = {};
    ss.save(input);
    ss.finish_save();
    ss.reset_load();

    uint32_t output = 0;

    ASSERT_TRUE(ss.load(output));
    EXPECT_EQ(input, output);
}

TEST(save_state, load_be64) {
    uint64_t input = 12345678;

    SaveState ss = {};
    ss.save(input);
    ss.finish_save();
    ss.reset_load();

    uint64_t output = 0;

    ASSERT_TRUE(ss.load(output));
    EXPECT_EQ(input, output);
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
