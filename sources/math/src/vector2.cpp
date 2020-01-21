#include <algorithm>
#include <cmath>
#include <limits>

#include <fundamental/debug.hpp>

#include "constants.hpp"
#include "utils.hpp"
#include "vector2.hpp"

namespace Argon::Math
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

float32 AR_VEC_CALL Vector2::getX(void) const
{
	return m_comps[0];
}

float32 AR_VEC_CALL Vector2::getY(void) const
{
	return m_comps[1];
}

void AR_VEC_CALL Vector2::setX(float32 x)
{
	m_comps[0] = x;
}

void AR_VEC_CALL Vector2::setY(float32 y)
{
	m_comps[1] = y;
}

void AR_VEC_CALL Vector2::set(float32 x, float32 y)
{
	m_comps[0] = x;
	m_comps[1] = y;
}

float32 AR_VEC_CALL Vector2::dot(const Vector2 &other) const
{
#ifdef AR_SIMD
	const Vector2 mult(_mm_mul_ps(m_data, other.m_data));

	return mult.m_comps[0] + mult.m_comps[1];
#else
	return m_comps[0] * other.m_comps[0] + m_comps[1] * other.m_comps[1];
#endif // ifdef AR_SIMD
}

float32 AR_VEC_CALL Vector2::getLength(void) const
{
	return std::sqrt(this->dot(*this));
}

float32 AR_VEC_CALL Vector2::getLengthSq(void) const
{
	return this->dot(*this);
}

Vector2& AR_VEC_CALL Vector2::normalize(void)
{
	AR_ASSERT_MSG(!isZeroEpsilon(), "Trying to normalize vector of length 0");
	return *this /= getLength();
}

Vector2 AR_VEC_CALL Vector2::getNormalized(void) const
{
	AR_ASSERT_MSG(!isZeroEpsilon(), "Trying to normalize vector of length 0");
	return *this / getLength();
}

void Vector2::zeroOut(void)
{
#ifdef AR_SIMD
	m_data = _mm_setzero_ps();
#else
	m_comps[0] = 0.f;
	m_comps[1] = 0.f;
#endif // ifdef AR_SIMD
}

bool Vector2::isZero(void) const
{
	return (*this == c_zero);
}

bool Vector2::isZeroEpsilon(void) const
{
	return this->getLengthSq() < c_epsilon * c_epsilon;
}

bool Vector2::hasInfinity(void) const
{
#ifdef AR_SIMD
	__m128 test = _mm_andnot_ps(Utils::c_SIMDZeroMask, m_data);
	const __m128 inf = _mm_set1_ps(std::numeric_limits<float32>::infinity());

	test = _mm_cmp_ps(test, inf, _CMP_EQ_OQ);

	return _mm_movemask_ps(test) != 0;
#else
	const float32 inf = std::numeric_limits<float32>::infinity();

	return (m_comps[0] == inf) || (m_comps[1] == inf);
#endif // ifdef AR_SIMD
}

Vector2 AR_VEC_CALL  Vector2::getAbs(void) const
{
#ifdef AR_SIMD
	return Vector2(_mm_and_ps(m_data, Utils::c_SIMDAbsMask));
#else
	return Vector2(std::abs(m_comps[0]), std::abs(m_comps[1]));
#endif // ifdef AR_SIMD
}

Vector2& AR_VEC_CALL Vector2::abs(void)
{
#ifdef AR_SIMD
	m_data = _mm_and_ps(m_data, Utils::c_SIMDAbsMask);
#else
	m_comps[0] = std::abs(m_comps[0]);
	m_comps[1] = std::abs(m_comps[1]);
#endif // ifdef AR_SIMD

	return *this;
}

Vector2 AR_VEC_CALL Vector2::getNegated(void) const
{
#ifdef AR_SIMD
	return Vector2(_mm_xor_ps(m_data, Utils::c_SIMDNegMask));
#else
	return Vector2(-m_comps[0], -m_comps[1]);
#endif // ifdef AR_SIMD
}

Vector2& AR_VEC_CALL Vector2::negate(void)
{
#ifdef AR_SIMD
	m_data = _mm_xor_ps(m_data, Utils::c_SIMDNegMask);
#else
	m_comps[0] = -m_comps[0];
	m_comps[1] = -m_comps[1];
#endif // ifdef AR_SIMD

	return *this;
}

float32 AR_VEC_CALL Vector2::getDistance(const Vector2 &other) const
{
	return (*this - other).getLength();
}

float32 AR_VEC_CALL Vector2::getDistanceSq(const Vector2 &other) const
{
	return (*this - other).getLengthSq();
}

int32 AR_VEC_CALL Vector2::getMaxAxis(void) const
{
	return m_comps[0] < m_comps[1] ? 1 : 0;
}

int32 AR_VEC_CALL Vector2::getMinAxis(void) const
{
	return m_comps[0] < m_comps[1] ? 0 : 1;
}

float32 AR_VEC_CALL Vector2::getMaxAxisValue(void) const
{
	return std::max(m_comps[0], m_comps[1]);
}

float32 AR_VEC_CALL Vector2::getMinAxisValue(void) const
{
	return std::min(m_comps[0], m_comps[1]);
}

Vector2 AR_VEC_CALL Vector2::lerp(const Vector2 &other, float32 t) const
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

Vector2 AR_VEC_CALL Vector2::getRotatedRad(float32 angleRad) const
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

Vector2& AR_VEC_CALL Vector2::rotateRad(float32 angleRad)
{
#ifdef AR_SIMD
	m_data = this->getRotatedRad(angleRad).m_data;
#else
	const float32 sinVal = std::sin(angleRad);
	const float32 cosVal = std::cos(angleRad);

	const float32 tempX = cosVal * m_comps[0] - sinVal * m_comps[1];
	m_comps[1] = sinVal * m_comps[0] + cosVal * m_comps[1];
	m_comps[0] = tempX;
#endif // ifdef AR_SIMD

	return *this;
}

Vector2 AR_VEC_CALL Vector2::getRotatedDeg(float32 angleDeg) const
{
	return this->getRotatedRad(angleDeg * c_rads_per_deg);
}

Vector2& AR_VEC_CALL Vector2::rotateDeg(float32 angleDeg)
{
	return this->rotateRad(angleDeg * c_rads_per_deg);
}

float32 AR_VEC_CALL Vector2::getAngle(const Vector2 &other) const
{
	const float32 scalar = std::sqrt(this->getLengthSq() * other.getLengthSq());
	AR_ASSERT_MSG(scalar != 0.f, "One of the vectors has length 0");
	return std::acos(this->dot(other) / scalar);
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
	return Vector2(_mm_xor_ps(m_data, Utils::c_SIMDNegMask));
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
} // namespace Argon::Math
