add_subdirectory(unit)
if (SAVE_STATE_BUILD_FUZZ_TESTS)
add_subdirectory(fuzz)
endif ()
