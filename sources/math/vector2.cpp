#include <cmath>
#include <limits>

#include <fundamental/debug.hpp>

#include "constants.hpp"
#include "utils.hpp"
#include "vector2.hpp"

namespace Argon
{
const Vector2 AR_ATTR_ALIGN(16) Vector2::c_zero = Vector2(0.f, 0.f);
const Vector2 AR_ATTR_ALIGN(16) Vector2::c_xAxis = Vector2(1.f, 0.f);
const Vector2 AR_ATTR_ALIGN(16) Vector2::c_yAxis = Vector2(0.f, 1.f);

Vector2::Vector2()
{
}

AR_VEC_CALL Vector2::Vector2(const Vector2 &) = default;

AR_VEC_CALL Vector2::Vector2(float32 x, float32 y)
{
#ifdef AR_SIMD
	m_data = _mm_set_ps(0.0, 0.0, y, x);
#else
	m_comps[0] = x;
	m_comps[1] = y;
	m_comps[2] = m_comps[3] = 0.f;
#endif // ifdef AR_SIMD
}

#ifdef AR_SIMD
AR_VEC_CALL Vector2::Vector2(__m128 data)
{
	m_data = data;
}
#endif // ifdef AR_SIMD

float32 AR_VEC_CALL Vector2::GetX(void) const
{
	return m_comps[0];
}

float32 AR_VEC_CALL Vector2::GetY(void) const
{
	return m_comps[1];
}

void AR_VEC_CALL Vector2::SetX(float32 x)
{
	m_comps[0] = x;
}

void AR_VEC_CALL Vector2::SetY(float32 y)
{
	m_comps[1] = y;
}

void AR_VEC_CALL Vector2::Set(float32 x, float32 y)
{
	m_comps[0] = x;
	m_comps[1] = y;
}

float32 AR_VEC_CALL Vector2::Dot(const Vector2 &other) const
{
#ifdef AR_SIMD
	const Vector2 mult(_mm_mul_ps(m_data, other.m_data));

	return mult.m_comps[0] + mult.m_comps[1];
#else
	return m_comps[0] * other.m_comps[0] + m_comps[1] * other.m_comps[1];
#endif // ifdef AR_SIMD
}

float32 AR_VEC_CALL Vector2::Length(void) const
{
	return std::sqrt(this->Dot(*this));
}

float32 AR_VEC_CALL Vector2::LengthSq(void) const
{
	return this->Dot(*this);
}

Vector2& AR_VEC_CALL Vector2::Normalize(void)
{
	AR_ASSERT_MSG(!IsZeroEpsilon(), "Trying to normalize vector of length 0");
	return *this /= Length();
}

Vector2 AR_VEC_CALL Vector2::Normalized(void) const
{
	AR_ASSERT_MSG(!IsZeroEpsilon(), "Trying to normalize vector of length 0");
	return *this / Length();
}

void Vector2::ZeroOut(void)
{
#ifdef AR_SIMD
	m_data = _mm_setzero_ps();
#else
	m_comps[0] = 0.f;
	m_comps[1] = 0.f;
#endif // ifdef AR_SIMD
}

bool Vector2::IsZero(void) const
{
	return (*this == c_zero);
}

bool Vector2::IsZeroEpsilon(void) const
{
	return this->LengthSq() < c_epsilon * c_epsilon;
}

bool Vector2::HasInfinity(void) const
{
#ifdef AR_SIMD
	__m128 test = _mm_andnot_ps(MathUtils::c_SIMDZeroMask, m_data);
	const __m128 inf = _mm_set1_ps(std::numeric_limits<float32>::infinity());

	test = _mm_cmp_ps(test, inf, _CMP_EQ_OQ);

	return _mm_movemask_ps(test) != 0;
#else
	const float32 inf = std::numeric_limits<float32>::infinity();

	return (m_comps[0] == inf) || (m_comps[1] == inf);
#endif // ifdef AR_SIMD
}

Vector2 AR_VEC_CALL  Vector2::GetAbs(void) const
{
#ifdef AR_SIMD
	return Vector2(_mm_and_ps(m_data, MathUtils::c_SIMDAbsMask));
#else
	return Vector2(std::abs(m_comps[0]), std::abs(m_comps[1]));
#endif // ifdef AR_SIMD
}

Vector2& AR_VEC_CALL Vector2::Abs(void)
{
#ifdef AR_SIMD
	m_data = _mm_and_ps(m_data, MathUtils::c_SIMDAbsMask);
#else
	m_comps[0] = std::abs(m_comps[0]);
	m_comps[1] = std::abs(m_comps[1]);
#endif // ifdef AR_SIMD

	return *this;
}

Vector2 AR_VEC_CALL Vector2::Negated(void) const
{
#ifdef AR_SIMD
	return Vector2(_mm_xor_ps(m_data, MathUtils::c_SIMDNegMask));
#else
	return Vector2(-m_comps[0], -m_comps[1]);
#endif // ifdef AR_SIMD
}

Vector2& AR_VEC_CALL Vector2::Negate(void)
{
#ifdef AR_SIMD
	m_data = _mm_xor_ps(m_data, MathUtils::c_SIMDNegMask);
#else
	m_comps[0] = -m_comps[0];
	m_comps[1] = -m_comps[1];
#endif // ifdef AR_SIMD

	return *this;
}

float32 AR_VEC_CALL Vector2::Distance(const Vector2 &other) const
{
	return (*this - other).Length();
}

float32 AR_VEC_CALL Vector2::DistanceSq(const Vector2 &other) const
{
	return (*this - other).LengthSq();
}

int32 AR_VEC_CALL Vector2::MaxAxis(void) const
{
	return m_comps[0] < m_comps[1] ? 1 : 0;
}

int32 AR_VEC_CALL Vector2::MinAxis(void) const
{
	return m_comps[0] < m_comps[1] ? 0 : 1;
}

float32 AR_VEC_CALL Vector2::MaxAxisValue(void) const
{
	return m_comps[0] < m_comps[1] ? m_comps[1] : m_comps[0];
}

float32 AR_VEC_CALL Vector2::MinAxisValue(void) const
{
	return m_comps[0] < m_comps[1] ? m_comps[0] : m_comps[1];
}

Vector2 AR_VEC_CALL Vector2::Lerp(const Vector2 &other, float32 t) const
{
#ifdef AR_SIMD
	__m128 mt = _mm_load_ss(&t);
	mt = _mm_shuffle_ps(mt, mt, 0x40); //_MM_SHUFFLE(1, 0, 0, 0);

	const __m128 lerpDir = _mm_sub_ps(other.m_data, m_data);
	const __m128 lerpMove = _mm_mul_ps(lerpDir, mt);

	return Vector2(_mm_add_ps(m_data, lerpMove));
#else
	const Vector2 lerpDir = other - *this;

	return (*this) + lerpDir * t;
#endif // ifdef AR_SIMD
}

Vector2 AR_VEC_CALL Vector2::RotatedRad(float32 angleRad) const
{
	const float32 sinVal = std::sin(angleRad);
	const float32 cosVal = std::cos(angleRad);

#ifdef AR_SIMD
	__m128 mSin = _mm_load_ss(&sinVal);
	mSin = _mm_shuffle_ps(mSin, mSin, 0x40);
	mSin = _mm_xor_ps(mSin, _mm_set_ps(+0.0, +0.0, -0.0, +0.0));

	__m128 mCos = _mm_load_ss(&cosVal);
	mCos = _mm_shuffle_ps(mCos, mCos, 0x40);

	const __m128 cosMult = _mm_mul_ps(m_data, mCos);
	__m128 sinMult = _mm_mul_ps(m_data, mSin);

	sinMult = _mm_shuffle_ps(sinMult, sinMult, 0x1);

	return Vector2(_mm_add_ps(cosMult, sinMult));
#else
	return Vector2(cosVal * m_comps[0] - sinVal * m_comps[1],
									sinVal * m_comps[0] + cosVal * m_comps[1]);
#endif // ifdef AR_SIMD
}

Vector2& AR_VEC_CALL Vector2::RotateRad(float32 angleRad)
{
#ifdef AR_SIMD
	m_data = this->RotatedRad(angleRad).m_data;
#else
	const float32 sinVal = std::sin(angleRad);
	const float32 cosVal = std::cos(angleRad);

	const float32 tempX = cosVal * m_comps[0] - sinVal * m_comps[1];
	m_comps[1] = sinVal * m_comps[0] + cosVal * m_comps[1];
	m_comps[0] = tempX;
#endif // ifdef AR_SIMD

	return *this;
}

Vector2 AR_VEC_CALL Vector2::RotatedDeg(float32 angleDeg) const
{
	return this->RotatedRad(angleDeg * c_rads_per_deg);
}

Vector2& AR_VEC_CALL Vector2::RotateDeg(float32 angleDeg)
{
	return this->RotateRad(angleDeg * c_rads_per_deg);
}

float32 AR_VEC_CALL Vector2::GetAngle(const Vector2 &other) const
{
	const float32 scalar = std::sqrt(this->LengthSq() * other.LengthSq());
	AR_ASSERT_MSG(scalar != 0.f, "One of the vectors has length 0");
	return std::acos(this->Dot(other) / scalar);
}

Vector2& AR_VEC_CALL Vector2::operator=(const Vector2 &other)
{
#ifdef AR_SIMD
	m_data = other.m_data;
#else
	m_comps[0] = other.m_comps[0];
	m_comps[1] = other.m_comps[1];
#endif // ifdef AR_SIMD

	return *this;
}

Vector2 AR_VEC_CALL Vector2::operator+(const Vector2 &other) const
{
#ifdef AR_SIMD
	return Vector2(_mm_add_ps(m_data, other.m_data));
#else
	return Vector2(m_comps[0] + other.m_comps[0], m_comps[1] + other.m_comps[1]);
#endif // ifdef AR_SIMD
}

Vector2& AR_VEC_CALL Vector2::operator+=(const Vector2 &other)
{
#ifdef AR_SIMD
	m_data = _mm_add_ps(m_data, other.m_data);
#else
	m_comps[0] += other.m_comps[0];
	m_comps[1] += other.m_comps[1];
#endif // ifdef AR_SIMD

	return *this;
}

Vector2 AR_VEC_CALL Vector2::operator-(void) const
{
#ifdef AR_SIMD
	return Vector2(_mm_xor_ps(m_data, MathUtils::c_SIMDNegMask));
#else
	return Vector2(-m_comps[0], -m_comps[1]);
#endif // ifdef AR_SIMD
}

Vector2 AR_VEC_CALL Vector2::operator-(const Vector2 &other) const
{
#ifdef AR_SIMD
	return Vector2(_mm_sub_ps(m_data, other.m_data));
#else
	return Vector2(m_comps[0] - other.m_comps[0], m_comps[1] - other.m_comps[1]);
#endif // ifdef AR_SIMD
}

Vector2& AR_VEC_CALL Vector2::operator-=(const Vector2 &other)
{
#ifdef AR_SIMD
	m_data = _mm_sub_ps(m_data, other.m_data);
#else
	m_comps[0] -= other.m_comps[0];
	m_comps[1] -= other.m_comps[1];
#endif // ifdef AR_SIMD

	return *this;
}

Vector2 AR_VEC_CALL Vector2::operator*(float32 scalar) const
{
#ifdef AR_SIMD
	return Vector2(_mm_mul_ps(m_data, _mm_set1_ps(scalar)));
#else
	return Vector2(m_comps[0] * scalar, m_comps[1] * scalar);
#endif // ifdef AR_SIMD
}

Vector2& AR_VEC_CALL Vector2::operator*=(float32 scalar)
{
#ifdef AR_SIMD
	m_data = _mm_mul_ps(m_data, _mm_set1_ps(scalar));
#else
	m_comps[0] *= scalar;
	m_comps[1] *= scalar;
#endif // ifdef AR_SIMD

	return *this;
}

Vector2 AR_VEC_CALL Vector2::operator/(float32 scalar) const
{
	AR_ASSERT_MSG(scalar != 0.f, "Scaling by 1/0");

#ifdef AR_SIMD
	return Vector2(_mm_div_ps(m_data, _mm_set1_ps(scalar)));
#else
	return Vector2(m_comps[0] / scalar, m_comps[1] / scalar);
#endif // ifdef AR_SIMD
}

Vector2& AR_VEC_CALL Vector2::operator/=(float32 scalar)
{
	AR_ASSERT_MSG(scalar != 0.f, "Scaling by 1/0");

#ifdef AR_SIMD
	m_data = _mm_div_ps(m_data, _mm_set1_ps(scalar));
#else
	m_comps[0] /= scalar;
	m_comps[1] /= scalar;
#endif // ifdef AR_SIMD

	return *this;
}

float32& AR_VEC_CALL Vector2::operator[](int32 i)
{
	AR_ASSERT_MSG(i >= 0 && i < 2, "Index is out of range");
	return m_comps[i];
}

const float32& AR_VEC_CALL Vector2::operator[](int32 i) const
{
	AR_ASSERT_MSG(i >= 0 && i < 2, "Index is out of range");
	return m_comps[i];
}

bool AR_VEC_CALL Vector2::operator==(const Vector2 &other) const
{
#ifdef AR_SIMD
	return 0xF == _mm_movemask_ps(_mm_cmp_ps(m_data, other.m_data, _CMP_EQ_OQ));
#else
	return (m_comps[0] == other.m_comps[0]) && (m_comps[1] == other.m_comps[1]);
#endif // ifdef AR_SIMD
}

bool AR_VEC_CALL Vector2::operator!=(const Vector2 &other) const
{
	return !(*this == other);
}

Vector2 AR_VEC_CALL operator*(float32 s, const Vector2 &v)
{
	return v * s;
}
} // namespace Argon
