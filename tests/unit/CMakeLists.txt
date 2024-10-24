add_executable(save_state_test save_state.cpp)
target_link_libraries(save_state_test
  GTest::gtest_main save_state)

gtest_discover_tests(save_state_test)
