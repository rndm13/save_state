# Extremely simple binary serialization

save_state is a C++ library for serializing and deserializing data into binary files

## Installation

Add this to your CMakeLists

```CMake
# Add Save State
# ===============

FetchContent_Declare(
  save_state
  GIT_REPOSITORY git@github.com:rndm13/save_state.git
  GIT_TAG "main"
)

set(SAVE_STATE_BUILD_TESTS, OFF)
set(SAVE_STATE_BUILD_SHARED, OFF)

FetchContent_MakeAvailable(save_state)

target_link_libraries(${PROJECT_NAME} PRIVATE save_state)
```

Then include the library like so:

```cpp

// This define is for your custom data versioning. Any time you change your format increment it.
#define SAVE_STATE_VERSION 0
#include "save_state.hpp"
```

## Usage

```cpp
struct TestData {
    std::variant<int32_t, std::string> id = 103;
    std::string name = "Sasha";
    std::string password = "12345678";
    std::vector<int64_t> points = {0, 2, 3, 5};
    std::optional<int64_t> average_points = 3;
    std::unordered_map<int32_t, std::string> dictionary = {{1, "one"}, {2, "two"}, {3, "three"}};

    // This macro accepts any number of pairs of a field name and a minimum version that field appears in
    // It defines 2 methods, `void save` and `bool load`. 
    // If you need to have a more sophisticated saving/loading mechanism you can define them yourself.
    SAVE_STATE_SAVE_IMPL(
            id, 0,
            name, 0,
            password, 0,
            points, 0,
            average_points, 0,
            dictionary, 0);
};
```

That's it! This makes it possible to serialize and deserialize your custom structs.
Now if you want to write to or read from file you just do:

```cpp
    TestData td = {};

    // Writing
    // ==============

    SaveState save{};
    save.save(td);
    save.finish_save();
    if (!save.write(out)) {
        Log(LogLevel::Error, "Failed to save");
    }
    

    // Reading
    // ==============
    std::ifstream in(filename);
    if (!in) {
        Log(LogLevel::Error, "Failed to load user config in %s", filename.c_str());
        return;
    }

    SaveState save{};
    if (!save.read(in) || !save.load(td) || save.load_idx != save.original_size) {
        Log(LogLevel::Error,
            "Failed to read user config in '%s', likely file is invalid or size exceeds maximum",
            filename.c_str());
    }
```
