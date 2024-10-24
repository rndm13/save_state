#pragma once

#include "cassert"
#include "cmath"
#include "cstring"
#include "optional"
#include "string"
#include "type_traits"
#include "unordered_map"
#include "variant"
#include "vector"

static constexpr std::string_view SAVE_STATE_FILE_HEADER = "SAVE_STATE";

static constexpr size_t SAVE_STATE_MAX_SIZE = 0x10000000;

#ifndef SAVE_STATE_VERSION
#define SAVE_STATE_VERSION 0
#endif

static constexpr size_t SAVE_STATE_LIB_VERSION = 0;

#define PARENS ()
#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(macro, ...) __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, a2, ...)                                                        \
    macro(a1, a2) __VA_OPT__(FOR_EACH_AGAIN PARENS(macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define TRY_LOAD(...)                                                                              \
    if (!this->load(__VA_ARGS__)) {                                                                \
        return false;                                                                              \
    }

#define SAVE_STATE_SAVE_FIELD(field_name, version)                                                 \
    if (version <= 0 || ss.save_version >= version) {                                              \
        ss.save(this->field_name);                                                                 \
    }

#define SAVE_STATE_LOAD_FIELD(field_name, version)                                                 \
    {                                                                                              \
        if (version <= 0 || ss.save_version >= version) {                                          \
            if (!ss.load(this->field_name)) {                                                      \
                return false;                                                                      \
            }                                                                                      \
        }                                                                                          \
    }

#define SAVE_STATE_RECOVER_LOAD_FIELD(field_name, version)                                         \
    {                                                                                              \
        if (version <= 0 || ss.save_version >= version) {                                          \
            if (!ss.load(this->field_name)) {                                                      \
                this->recover();                                                                   \
                return false;                                                                      \
            }                                                                                      \
        }                                                                                          \
    }

#define SAVE_STATE_SAVE_IMPL(...)                                                                  \
    inline void save(SaveState& ss) const {                                                        \
        FOR_EACH(SAVE_STATE_SAVE_FIELD, __VA_ARGS__)                                               \
    }                                                                                              \
    inline bool load(SaveState& ss) {                                                              \
        FOR_EACH(SAVE_STATE_LOAD_FIELD, __VA_ARGS__)                                               \
        return true;                                                                               \
    }

#define SAVE_STATE_RECOVER_SAVE_IMPL(...)                                                          \
    inline void save(SaveState& ss) const {                                                        \
        FOR_EACH(SAVE_STATE_SAVE_FIELD, __VA_ARGS__)                                               \
    }                                                                                              \
    inline bool load(SaveState& ss) {                                                              \
        FOR_EACH(SAVE_STATE_LOAD_FIELD, __VA_ARGS__)                                               \
        return true;                                                                               \
    }

#define SAVE_STATE_DEF_PRE_SAVE_FIELD(field_type, field_name)                                      \
    static std::optional<field_type> __pre_save_##field_name = std::nullopt;

#define SAVE_STATE_RECOVER_FIELD(field_type, field_name)                                           \
    if (__pre_save_##field_name.has_value()) {                                                     \
        this->field_name = __pre_save_##field_name.value();                                        \
        __pre_save_##field_name = std::nullopt;                                                    \
    } else {                                                                                       \
        __pre_save_##field_name = this->field_name;                                                \
    }

#define SAVE_STATE_POST_LOAD_FIELD(field_type, field_name) __pre_save_##field_name = std::nullopt;

#define SAVE_STATE_RECOVER_IMPL(...)                                                               \
    inline void recover() {                                                                        \
        FOR_EACH(SAVE_STATE_DEF_PRE_SAVE_FIELD, __VA_ARGS__)                                       \
        FOR_EACH(SAVE_STATE_RECOVER_FIELD, __VA_ARGS__)                                            \
    }

namespace details {
template <class Variant>
constexpr bool valid_variant_from_index(size_t index) {
    return index < std::variant_size_v<Variant>;
}

template <class Variant, size_t I = 0>
constexpr std::optional<Variant> variant_from_index(size_t index) {
    if constexpr (valid_variant_from_index<Variant>(I)) {
        return index == 0 ? Variant{std::in_place_index<I>}
                          : variant_from_index<Variant, I + 1>(index - 1);
    }

    return std::nullopt;
}
}; // namespace details

struct SaveState {
    size_t save_lib_version = SAVE_STATE_LIB_VERSION;
    size_t save_version = SAVE_STATE_VERSION;

    size_t original_size = 0;
    size_t load_idx = 0;
    std::vector<char> buffer;

    // Helpers
    template <class T = void>
    void save(const char* ptr, size_t size = sizeof(T)) {
        assert(ptr);
        if (this->buffer.capacity() < this->buffer.size() + size) {
            this->buffer.reserve(this->buffer.capacity() * 2);
        }

        size_t orig_size = this->buffer.size();

        this->buffer.resize(orig_size + size);

        memcpy(this->buffer.data() + orig_size, ptr, size);
    }

    template <class T = void>
    bool load(char* ptr, size_t size = sizeof(T)) {
        assert(ptr);

        if (this->load_idx + size > buffer.size()) {
            return false;
        }

        const char* begin = this->buffer.data() + this->load_idx;

        memcpy(ptr, begin, size);
        this->load_idx += size;

        return true;
    }

    template <class T, std::enable_if_t<std::is_trivially_copyable<T>::value, int> = 0>
    void save(T trivial) {
        this->save<T>(reinterpret_cast<char*>(&trivial));
    }

    template <class T, std::enable_if_t<std::is_trivially_copyable<T>::value, int> = 0>
    bool load(T& trivial) {
        return this->load(reinterpret_cast<char*>(&trivial), sizeof(T));
    }

    void save(const std::string& str);
    bool load(std::string& str);

    void save(const std::monostate&) {}
    bool load(std::monostate&) {
        return true;
    }

    template <class T>
    void save(const std::optional<T>& opt) {
        bool has_value = opt.has_value();
        this->save(has_value);

        if (has_value) {
            this->save(opt.value());
        }
    }

    template <class T>
    bool load(std::optional<T>& opt) {
        bool has_value;
        TRY_LOAD(has_value);

        if (has_value) {
            opt = T{};
            TRY_LOAD(opt.value());
        } else {
            opt = std::nullopt;
        }

        return true;
    }

    template <class K, class V>
    void save(const std::unordered_map<K, V>& map) {
        size_t size = map.size();
        this->save(size);

        for (const auto& [k, v] : map) {
            this->save(k);
            this->save(v);
        }
    }

    template <class K, class V>
    bool load(std::unordered_map<K, V>& map) {
        size_t size;
        TRY_LOAD(size);

        if (size > SAVE_STATE_MAX_SIZE) {
            return false;
        }

        map.clear();
        for (size_t i = 0; i < size; i++) {
            K k;
            V v;
            TRY_LOAD(k);
            TRY_LOAD(v);

            map.insert_or_assign(k, v);
        }

        return true;
    }

    template <class... T>
    void save(const std::variant<T...>& variant) {
        assert(variant.index() != std::variant_npos);
        size_t index = variant.index();
        this->save(index);

        std::visit([this](const auto& s) { this->save(s); }, variant);
    }

    template <class... T>
    bool load(std::variant<T...>& variant) {
        size_t index;
        TRY_LOAD(index);

        auto variant_opt = details::variant_from_index<std::variant<T...>>(index);

        if (!variant_opt.has_value()) {
            return false;
        }

        variant = variant_opt.value();

        return std::visit([this](auto& s) { return this->load(s); }, variant);
    }

    template <class Element>
    void save(const std::vector<Element>& vec) {
        size_t size = vec.size();
        this->save(size);

        for (const auto& elem : vec) {
            this->save(elem);
        }
    }

    template <class Element>
    bool load(std::vector<Element>& vec) {
        size_t size;
        TRY_LOAD(size);

        if (size > SAVE_STATE_MAX_SIZE) {
            return false;
        }

        vec.clear();
        vec.reserve(size);
        vec.resize(size);

        for (auto& elem : vec) {
            TRY_LOAD(elem);
        }

        return true;
    }

    // YOU HAVE TO BE CAREFUL NOT TO PASS POINTERS!
    template <class T, std::enable_if_t<!std::is_trivially_copyable<T>::value, int> = 0>
    void save(const T& any) {
        any.save(*this);
    }

    // YOU HAVE TO BE CAREFUL NOT TO PASS POINTERS!
    template <class T, std::enable_if_t<!std::is_trivially_copyable<T>::value, int> = 0>
    bool load(T& any) {
        return any.load(*this);
    }

    void finish_save();

    void reset_load();

    // Returns false when failed
    bool write(std::ostream& os) const;

    // Returns false when failed
    bool read(std::istream& is);
};

struct UndoHistory {
    size_t undo_idx = 0;
    std::vector<SaveState> undo_history = {};

    // should be called after every edit
    template <class T>
    void push_undo_history(const T& obj) {
        assert(obj);

        if (this->undo_idx + 1 < this->undo_history.size()) {
            // Remove redos
            this->undo_history.resize(this->undo_idx + 1);
        }

        SaveState& new_save = this->undo_history.emplace_back();
        new_save.save(*obj);
        new_save.finish_save();

        this->undo_idx = this->undo_history.size() - 1;
    }

    template <class T>
    void reset_undo_history(const T& obj) {
        // add initial undo
        this->undo_history.clear();
        this->undo_idx = 0;
        this->push_undo_history(obj);
    }

    constexpr bool can_undo() const {
        return this->undo_idx > 0;
    }

    template <class T>
    bool undo(T& obj) {
        if (!this->can_undo()) {
            return false;
        }

        this->undo_idx--;
        if (!this->undo_history[this->undo_idx].load(obj)) {
            this->undo_idx++;
            return false;
        }

        this->undo_history[this->undo_idx].reset_load();
        return true;
    }

    bool can_redo() const {
        return this->undo_idx < this->undo_history.size() - 1;
    }

    template <class T>
    bool redo(T& obj) {
        if (!this->can_redo()) {
            return false;
        }

        this->undo_idx++;
        if (!this->undo_history[this->undo_idx].load(obj)) {
            this->undo_idx--;
            return false;
        }

        this->undo_history[this->undo_idx].reset_load();
        return true;
    }

    UndoHistory() {}
    template <class T>
    UndoHistory(const T& obj) {
        reset_undo_history(obj);
    }

    UndoHistory(const UndoHistory&) = default;
    UndoHistory(UndoHistory&&) = default;
    UndoHistory& operator=(const UndoHistory&) = default;
    UndoHistory& operator=(UndoHistory&&) = default;
};
