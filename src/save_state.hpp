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
#include "string_view"

static constexpr std::string_view SAVE_STATE_FILE_HEADER = "SAVE_STATE";

static constexpr size_t SAVE_STATE_MAX_SIZE = 0x10000000;

#ifndef SAVE_STATE_VERSION
#define SAVE_STATE_VERSION 0
#endif

static constexpr size_t SAVE_STATE_LIB_VERSION = 0;

#define FE_CALLITn01(a,b)  a b
#define FE_CALLITn02(a,b)  a b
#define FE_CALLITn03(a,b)  a b 
#define FE_CALLITn04(a,b)  a b
#define FE_CALLITn04(a,b)  a b
#define FE_CALLITn05(a,b)  a b
#define FE_CALLITn06(a,b)  a b
#define FE_CALLITn07(a,b)  a b
#define FE_CALLITn08(a,b)  a b
#define FE_CALLITn09(a,b)  a b
#define FE_CALLITn10(a,b)  a b
#define FE_CALLITn11(a,b)  a b
#define FE_CALLITn12(a,b)  a b
#define FE_CALLITn13(a,b)  a b
#define FE_CALLITn14(a,b)  a b
#define FE_CALLITn15(a,b)  a b
#define FE_CALLITn16(a,b)  a b
#define FE_CALLITn17(a,b)  a b
#define FE_CALLITn18(a,b)  a b
#define FE_CALLITn19(a,b)  a b
#define FE_CALLITn20(a,b)  a b
#define FE_CALLITn21(a,b)  a b

/* the MSVC preprocessor expands __VA_ARGS__ as a single argument, so it needs
 * to be expanded indirectly through the CALLIT macros.
 * http://connect.microsoft.com/VisualStudio/feedback/details/380090/variadic-macro-replacement
 * http://stackoverflow.com/questions/21869917/visual-studio-va-args-issue
 */
#define FE_n00()
#define FE_n01(what, a, b, ...)  what(a, b)
#define FE_n02(what, a, b, ...)  what(a, b) FE_CALLITn02(FE_n01,(what, ##__VA_ARGS__))
#define FE_n03(what, a, b, ...)  what(a, b) FE_CALLITn03(FE_n02,(what, ##__VA_ARGS__))
#define FE_n04(what, a, b, ...)  what(a, b) FE_CALLITn04(FE_n03,(what, ##__VA_ARGS__))
#define FE_n05(what, a, b, ...)  what(a, b) FE_CALLITn05(FE_n04,(what, ##__VA_ARGS__))
#define FE_n06(what, a, b, ...)  what(a, b) FE_CALLITn06(FE_n05,(what, ##__VA_ARGS__))
#define FE_n07(what, a, b, ...)  what(a, b) FE_CALLITn07(FE_n06,(what, ##__VA_ARGS__))
#define FE_n08(what, a, b, ...)  what(a, b) FE_CALLITn08(FE_n07,(what, ##__VA_ARGS__))
#define FE_n09(what, a, b, ...)  what(a, b) FE_CALLITn09(FE_n08,(what, ##__VA_ARGS__))
#define FE_n10(what, a, b, ...)  what(a, b) FE_CALLITn10(FE_n09,(what, ##__VA_ARGS__))
#define FE_n11(what, a, b, ...)  what(a, b) FE_CALLITn11(FE_n10,(what, ##__VA_ARGS__))
#define FE_n12(what, a, b, ...)  what(a, b) FE_CALLITn12(FE_n11,(what, ##__VA_ARGS__))
#define FE_n13(what, a, b, ...)  what(a, b) FE_CALLITn13(FE_n12,(what, ##__VA_ARGS__))
#define FE_n14(what, a, b, ...)  what(a, b) FE_CALLITn14(FE_n13,(what, ##__VA_ARGS__))
#define FE_n15(what, a, b, ...)  what(a, b) FE_CALLITn15(FE_n14,(what, ##__VA_ARGS__))
#define FE_n16(what, a, b, ...)  what(a, b) FE_CALLITn16(FE_n15,(what, ##__VA_ARGS__))
#define FE_n17(what, a, b, ...)  what(a, b) FE_CALLITn17(FE_n16,(what, ##__VA_ARGS__))
#define FE_n18(what, a, b, ...)  what(a, b) FE_CALLITn18(FE_n17,(what, ##__VA_ARGS__))
#define FE_n19(what, a, b, ...)  what(a, b) FE_CALLITn19(FE_n18,(what, ##__VA_ARGS__))
#define FE_n20(what, a, b, ...)  what(a, b) FE_CALLITn20(FE_n19,(what, ##__VA_ARGS__))
#define FE_n21(what, a, b, ...)  what(a, b) FE_CALLITn21(FE_n20,(what, ##__VA_ARGS__))
#define FE_n22(...)           ERROR: FOR_EACH only supports up to 21 arguments

#define FE_GET_MACRO(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,NAME,...) NAME
#define FOR_EACH(what, ...) FE_CALLITn01(FE_GET_MACRO(_0, ((##__VA_ARGS__) / 2),FE_n22,FE_n21,FE_n20,FE_n19, \
                            FE_n18,FE_n17,FE_n16,FE_n15,FE_n14,FE_n13,FE_n12,FE_n11,FE_n10,FE_n09,\
                            FE_n08,FE_n07,FE_n06,FE_n05,FE_n04,FE_n03,FE_n02,FE_n01,FE_n00), (what, ##__VA_ARGS__))

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
