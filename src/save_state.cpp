#include "save_state.hpp"
#include "cassert"
#include "cstdint"
#include "iostream"

void SaveState::save_buf(const char* ptr, size_t size) {
    assert(ptr);

    size_t new_capacity = this->buffer.capacity() == 0 ? 1 : this->buffer.capacity();
    while (new_capacity < this->buffer.size() + size && new_capacity < SAVE_STATE_MAX_SIZE) {
        new_capacity *= 2;
    }
    this->buffer.reserve(new_capacity);

    size_t orig_size = this->buffer.size();

    this->buffer.resize(orig_size + size);

    memcpy(this->buffer.data() + orig_size, ptr, size);
}

bool SaveState::load_buf(char* ptr, size_t size) {
    assert(ptr);

    if (this->load_idx + size > buffer.size()) {
        return false;
    }

    const char* begin = this->buffer.data() + this->load_idx;

    memcpy(ptr, begin, size);
    this->load_idx += size;

    return true;
}

void SaveState::save(const std::string& str) {
    size_t length = str.length();
    this->save(length);
    this->save_buf(str.data(), str.length());
    return;
}

bool SaveState::load(std::string& str) {
    size_t length;
    TRY_LOAD(length);

    if (length > SAVE_STATE_MAX_SIZE) {
        return false;
    }

    if (length > 0) {
        str.reserve(length);
        str.resize(length);

        if (!this->load_buf(str.data(), length)) {
            return false;
        }
    }

    return true;
}

void SaveState::finish_save() {
    this->original_size = this->buffer.size();
    this->buffer.shrink_to_fit();
}

void SaveState::reset_load() {
    this->load_idx = 0;
}

bool SaveState::write(std::ostream& os) const {
    assert(os);
    assert(this->original_size > 0);
    assert(this->buffer.size() == this->original_size);

    if (this->original_size > SAVE_STATE_MAX_SIZE) {
        return false;
    }

    os.write(SAVE_STATE_FILE_HEADER.data(), SAVE_STATE_FILE_HEADER.size());

    os.write(
        reinterpret_cast<const char*>(&this->save_lib_version), sizeof(this->save_lib_version));
    os.write(reinterpret_cast<const char*>(&this->save_version), sizeof(this->save_version));

    os.write(reinterpret_cast<const char*>(&this->original_size), sizeof(this->original_size));

    os.write(this->buffer.data(), static_cast<int32_t>(this->buffer.size()));
    os.flush();

    return true;
}

bool SaveState::read(std::istream& is) {
    assert(is);

    auto read_bytes = [&is](auto& val, size_t size = 0) {
        if (size == 0) {
            size = sizeof(val);
        }
        is.read(reinterpret_cast<char*>(&val), size);

        return is && !is.eof();
    };

    char save_str[SAVE_STATE_FILE_HEADER.size() + 1] = {};
    if (!read_bytes(save_str, SAVE_STATE_FILE_HEADER.size())) {
        return false;
    }
    if (memcmp(save_str, SAVE_STATE_FILE_HEADER.data(), SAVE_STATE_FILE_HEADER.size()) != 0) {
        return false;
    }

    if (!read_bytes(this->save_lib_version)) {
        return false;
    }
    if (!read_bytes(this->save_version)) {
        return false;
    }
    if (!read_bytes(this->original_size)) {
        return false;
    }
    if (this->original_size > SAVE_STATE_MAX_SIZE || this->original_size <= 0) {
        return false;
    }

    this->buffer.reserve(this->original_size);
    this->buffer.resize(this->original_size);

    is.read(this->buffer.data(), static_cast<int32_t>(this->original_size));

    return true;
}

void SaveState::save(const std::monostate&) {}
bool SaveState::load(std::monostate&) {
    return true;
}
