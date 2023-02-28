#pragma once
#include <bitset>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <utility>  // For std::pair, used by loadROMWithHash
#include <vector>

#include "fmt/color.h"   // Text coloring fmt functions
#include "fmt/format.h"  // Core fmt functions

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

template <typename... Args>
[[noreturn]] static void panic(const char* fmt, const Args&... args) {
    fmt::print(fg(fmt::color::red), fmt, args...);
    fmt::print(fg(fmt::color::red), "Aborting...\n");
    exit(1);
}

template <typename... Args>
static void panic(bool condition, const char* fmt, const Args&... args) {
    if (condition) {
        fmt::print(fg(fmt::color::red), fmt, args...);
        fmt::print(fg(fmt::color::red), "Aborting...\n");
        exit(1);
    }
}