#pragma once

#include <array>
#include <optional>
#include <string_view>

// mathematical constants
inline constexpr double PI = 3.14159265358979323846;
inline constexpr double E  = 2.71828182845904523536;

// entry for builtin function table
struct FuncEntry {
    std::string_view name;
    int arity; // сколько аргументов принимает
};

// all 14 builtin functions
inline constexpr std::array<FuncEntry, 14> BUILTIN_FUNCTIONS = {{
    {"sin",   1},
    {"cos",   1},
    {"tan",   1},
    {"asin",  1},
    {"acos",  1},
    {"atan",  1},
    {"log",   1},  // log base 10
    {"ln",    1},  // natural log
    {"sqrt",  1},
    {"abs",   1},
    {"exp",   1},
    {"ceil",  1},
    {"floor", 1},
    {"sign",  1},
}};

// ищет функцию по имени в таблице
constexpr std::optional<FuncEntry> findFunction(std::string_view name) {
    for (std::size_t i = 0; i < BUILTIN_FUNCTIONS.size(); i++) {
        if (BUILTIN_FUNCTIONS[i].name == name) {
            return BUILTIN_FUNCTIONS[i];
        }
    }
    return std::nullopt;
}

// entry for constants
struct ConstEntry {
    std::string_view name;
    double value;
};

inline constexpr std::array<ConstEntry, 2> BUILTIN_CONSTANTS = {{
    {"pi", PI},
    {"e",  E},
}};

// ищем константу
constexpr std::optional<double> findConstant(std::string_view name) {
    for (std::size_t i = 0; i < BUILTIN_CONSTANTS.size(); i++) {
        if (BUILTIN_CONSTANTS[i].name == name) {
            return BUILTIN_CONSTANTS[i].value;
        }
    }
    return std::nullopt;
}
