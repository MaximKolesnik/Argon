#pragma once

#include <fundamental/compiler_macros.hpp>
#include <fundamental/types.hpp>

#include "forward_declarations.hpp"
#include "simd.hpp"

namespace argon::math
{
namespace utils
{
#ifdef AR_SIMD
const __m128 c_SIMDZeroMask = _mm_set_ps(-0.f, -0.f, -0.f, -0.f);
const __m128 c_SIMDAbsMask  = _mm_castsi128_ps(
																_mm_set_epi32(0x7FFFFFFF, 0x7FFFFFFF,
																							0x7FFFFFFF, 0x7FFFFFFF));
const __m128 c_SIMDNegMask  = _mm_castsi128_ps(
																_mm_set_epi32(static_cast<int>(0x80000000),
																							static_cast<int>(0x80000000),
																							static_cast<int>(0x80000000),
																							static_cast<int>(0x80000000)));
#endif // ifdef AR_SIMD

template<class T>
T getSign(const T& val)
{
	return (val >= T(0)) ? T(1) : T(-1);
}

float32 round(float32 val, uint16 numDecimals);

void gramSchmidt(Vector3 &e1, Vector3 &e2, Vector3 &e3);
} // namespace utils
} // namespace argon::math
