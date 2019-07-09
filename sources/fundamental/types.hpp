#pragma once

#include <cstdint>
#include <cstddef>

namespace Argon
{
using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using intptr = std::intptr_t;
using uintptr = std::uintptr_t;

using float32 = float;
using float64 = double;

using byte = std::byte;
using sizet = std::size_t;

static_assert(sizeof(int8) == 1, "int8 must be 8 bits");
static_assert(sizeof(int16) == 2, "int16 must be 16 bits");
static_assert(sizeof(int32) == 4, "int32 must be 32 bits");
static_assert(sizeof(int64) == 8, "int64 must be 64 bits");

static_assert(sizeof(uint8) == 1, "uint8 must be 8 bits");
static_assert(sizeof(uint16) == 2, "uint16 must be 16 bits");
static_assert(sizeof(uint32) == 4, "uint32 must be 32 bits");
static_assert(sizeof(uint64) == 8, "uint64 must be 64 bits");

static_assert(sizeof(intptr) == 8, "intptr must be 64 bits");
static_assert(sizeof(uintptr) == 8, "uintptr must be 64 bits");

static_assert(sizeof(float32) == 4, "float32 must be 32 bits");
static_assert(sizeof(float64) == 8, "float64 must be 64 bits");
} // namespace Argon
