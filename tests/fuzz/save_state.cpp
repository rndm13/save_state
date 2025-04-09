#include "../../src/save_state.hpp"
#include "../save_state_test_data.hpp"

#include <cstdint>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    TestData td = {};
    SaveState save = {};

    save.save_buf(reinterpret_cast<const char*>(Data), Size);
    save.finish_save();
    save.reset_load();

    if (!save.load(td) || save.load_idx != save.original_size) {
        return -1;
    }

    return 0;
}
