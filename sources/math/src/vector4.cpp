#include <cmath>

#include <fundamental/debug.hpp>

#include "constants.hpp"
#include "utils.hpp"
#include "vector3.hpp"
#include "vector4.hpp"

namespace Argon::Math
{
const Vector4 AR_ATTR_ALIGN(16) Vector4::c_zero(0.0, 0.0, 0.0, 0.0);
const Vector4 AR_ATTR_ALIGN(16) Vector4::c_xAxis(1.0, 0.0, 0.0, 0.0);
const Vector4 AR_ATTR_ALIGN(16) Vector4::c_yAxis(0.0, 1.0, 0.0, 0.0);
const Vector4 AR_ATTR_ALIGN(16) Vector4::c_zAxis(0.0, 0.0, 1.0, 0.0);
const Vector4 AR_ATTR_ALIGN(16) Vector4::c_wAxis(0.0, 0.0, 0.0, 1.0);

Vector4::Vector4()
{
}

AR_VEC_CALL Vector4::Vector4(const Vector4 &) = default;

AR_VEC_CALL Vector4::Vector4(float32 x, float32 y, float32 z, float32 w)
{
#ifdef AR_SIMD
	m_data = _mm_set_ps(w, z, y, x);
#else
	m_comps[0] = x, m_comps[1] = y, m_comps[2] = z, m_comps[3] = w;
#endif // ifdef AR_SIMD
}

Vector4& AR_VEC_CALL Vector4::operator=(const Vector4 &other)
{
#ifdef AR_SIMD
	m_data = other.m_data;
#else
	m_comps[0] = other.m_comps[0];
	m_comps[1] = other.m_comps[1];
	m_comps[2] = other.m_comps[2];
	m_comps[3] = other.m_comps[3];
#endif // ifdef AR_SIMD

	return *this;
}

#ifdef AR_SIMD
AR_VEC_CALL Vector4::Vector4(__m128 data)
{
	m_data = data;
}

__m128 AR_VEC_CALL Vector4::get128(void) const
{
	return m_data;
}

void AR_VEC_CALL Vector4::set(__m128 data)
{
	m_data = data;
}
#endif // ifdef AR_SIMD

float32 AR_VEC_CALL Vector4::getX() const
{
	return m_comps[0];
}

float32 AR_VEC_CALL Vector4::getY() const
{
	return m_comps[1];
}

float32 AR_VEC_CALL Vector4::getZ() const
{
	return m_comps[2];
}

float32 AR_VEC_CALL Vector4::getW() const
{
	return m_comps[3];
}

void AR_VEC_CALL Vector4::setX(float32 x)
{
	m_comps[0] = x;
}

void AR_VEC_CALL Vector4::setY(float32 y)
{
	m_comps[1] = y;
}

void AR_VEC_CALL Vector4::setZ(float32 z)
{
	m_comps[2] = z;
}

void AR_VEC_CALL Vector4::setW(float32 w)
{
	m_comps[3] = w;
}

void AR_VEC_CALL Vector4::set(float32 x, float32 y, float32 z, float32 w)
{
#ifdef AR_SIMD
	m_data = _mm_set_ps(w, z, y, x);
#else
	m_comps[0] = x, m_comps[1] = y, m_comps[2] = z, m_comps[3] = w;
#endif // ifdef AR_SIMD
}

void Vector4::zeroOut(void)
{
#ifdef AR_SIMD
	m_data = _mm_setzero_ps();
#else
	m_comps[0] = m_comps[1] = m_comps[2] = m_comps[3] = 0.f;
#endif // ifdef AR_SIMD
}

float32 AR_VEC_CALL Vector4::dot(const Vector4 &other) const
{
#ifdef AR_SIMD
	__m128 m = _mm_mul_ps(m_data, other.m_data);
	const __m128 w = _mm_shuffle_ps(m, m, 0xFF); //_MM_SHUFFLE(3, 3, 3, 3)
	const __m128 z = _mm_shuffle_ps(m, m, 0xAA); //_MM_SHUFFLE(2, 2, 2, 2)
	const __m128 y = _mm_shuffle_ps(m, m, 0x55); //_MM_SHUFFLE(1, 1, 1, 1)

	m = _mm_add_ps(m, w);
	m = _mm_add_ps(m, z);
	m = _mm_add_ps(m, y);

	return _mm_cvtss_f32(m);
#else
	return m_comps[0] * other.m_comps[0] + m_comps[1] * other.m_comps[1]
		+ m_comps[2] * other.m_comps[2] + m_comps[3] * other.m_comps[3];
#endif // ifdef AR_SIMD
}

float32 AR_VEC_CALL Vector4::dot(const Vector3 &other) const
{
#ifdef AR_SIMD
	const __m128 v = _mm_set_ps(1.0, other[2], other[1], other[0]);
	__m128 m = _mm_mul_ps(m_data, v);
	const __m128 w = _mm_shuffle_ps(m, m, 0xFF); //_MM_SHUFFLE(3, 3, 3, 3)
	const __m128 z = _mm_shuffle_ps(m, m, 0xAA); //_MM_SHUFFLE(2, 2, 2, 2)
	const __m128 y = _mm_shuffle_ps(m, m, 0x55); //_MM_SHUFFLE(1, 1, 1, 1)

	m = _mm_add_ps(m, w);
	m = _mm_add_ps(m, z);
	m = _mm_add_ps(m, y);

	return _mm_cvtss_f32(m);
#else
	return m_comps[0] * other[0] + m_comps[1] * other[1]
		+ m_comps[2] * other[2] + m_comps[3];
#endif // ifdef AR_SIMD
}

float32 AR_VEC_CALL Vector4::dot3(const Vector3 &other) const
{
#ifdef AR_SIMD
	__m128 m = _mm_mul_ps(m_data, other.get128());
	const __m128 z = _mm_shuffle_ps(m, m, 0xAA); //_MM_SHUFFLE(2, 2, 2, 2)
	const __m128 y = _mm_shuffle_ps(m, m, 0x55); //_MM_SHUFFLE(1, 1, 1, 1)

	m = _mm_add_ps(m, z);
	m = _mm_add_ps(m, y);

	return _mm_cvtss_f32(m);
#else
	return m_comps[0] * other[0] + m_comps[1] * other[1]
		+ m_comps[2] * other[2];
#endif // ifdef AR_SIMD
}

float32 AR_VEC_CALL Vector4::getLength(void) const
{
	return std::sqrt(this->dot(*this));
}

float32 AR_VEC_CALL Vector4::getLengthSq(void) const
{
	return this->dot(*this);
}

Vector4& AR_VEC_CALL Vector4::normalize(void)
{
	AR_ASSERT_MSG(!isZeroEpsilon(), "Trying to normalize vector of length 0");
	return *this /= getLength();
}

Vector4 AR_VEC_CALL Vector4::getNormalized(void) const
{
	AR_ASSERT_MSG(!isZeroEpsilon(), "Trying to normalize vector of length 0");
	return *this / getLength();
}

bool Vector4::isZeroEpsilon(void) const
{
	return getLengthSq() < c_epsilon * c_epsilon;
}

Vector4 AR_VEC_CALL Vector4::operator+(const Vector4 &other) const
{
#ifdef AR_SIMD
	return Vector4(_mm_add_ps(m_data, other.m_data));
#else
	return Vector4(m_comps[0] + other.m_comps[0], m_comps[1] + other.m_comps[1],
		m_comps[2] + other.m_comps[2], m_comps[3] + other.m_comps[3]);
#endif // ifdef AR_SIMD
}

Vector4& AR_VEC_CALL Vector4::operator+=(const Vector4 &other)
{
#ifdef AR_SIMD
	m_data = _mm_add_ps(m_data, other.m_data);
#else
	m_comps[0] += other.m_comps[0];
	m_comps[1] += other.m_comps[1];
	m_comps[2] += other.m_comps[2];
	m_comps[3] += other.m_comps[3];
#endif // ifdef AR_SIMD

	return *this;
}

Vector4 AR_VEC_CALL Vector4::operator-(void) const
{
#ifdef AR_SIMD
	return Vector4(_mm_xor_ps(m_data, Utils::c_SIMDNegMask));
#else
	return Vector4(-m_comps[0], -m_comps[1], -m_comps[2], -m_comps[3]);
#endif // ifdef AR_SIMD
}

Vector4 AR_VEC_CALL Vector4::operator-(const Vector4 &other) const
{
#ifdef AR_SIMD
	return Vector4(_mm_sub_ps(m_data, other.m_data));
#else
	return Vector4(m_comps[0] - other.m_comps[0], m_comps[1] - other.m_comps[1],
		m_comps[2] - other.m_comps[2], m_comps[3] - other.m_comps[3]);
#endif // ifdef AR_SIMD
}

Vector4& AR_VEC_CALL Vector4::operator-=(const Vector4 &other)
{
#ifdef AR_SIMD
	m_data = _mm_sub_ps(m_data, other.m_data);
#else
	m_comps[0] -= other.m_comps[0];
	m_comps[1] -= other.m_comps[1];
	m_comps[2] -= other.m_comps[2];
	m_comps[3] -= other.m_comps[3];
#endif // ifdef AR_SIMD

	return *this;
}

Vector4 AR_VEC_CALL Vector4::operator*(float32 scalar) const
{
#ifdef AR_SIMD
	return Vector4(_mm_mul_ps(m_data, _mm_set1_ps(scalar)));
#else
	return Vector4(m_comps[0] * scalar, m_comps[1] * scalar,
		m_comps[2] * scalar, m_comps[3] * scalar);
#endif // ifdef AR_SIMD
}

Vector4& AR_VEC_CALL Vector4::operator*=(float32 scalar)
{
#ifdef AR_SIMD
	m_data = _mm_mul_ps(m_data, _mm_set1_ps(scalar));
#else
	m_comps[0] *= scalar;
	m_comps[1] *= scalar;
	m_comps[2] *= scalar;
	m_comps[3] *= scalar;
#endif // ifdef AR_SIMD

	return *this;
}

Vector4 AR_VEC_CALL Vector4::operator/(float32 scalar) const
{
	AR_ASSERT_MSG(scalar != 0.f, "Scaling by 1/0");
#ifdef AR_SIMD
	return Vector4(_mm_div_ps(m_data, _mm_set1_ps(scalar)));
#else
	return Vector4(m_comps[0] / scalar, m_comps[1] / scalar,
		m_comps[2] / scalar, m_comps[3] / scalar);
#endif // ifdef AR_SIMD
}

Vector4& AR_VEC_CALL Vector4::operator/=(float32 scalar)
{
	AR_ASSERT_MSG(scalar != 0.f, "Scaling by 1/0");
#ifdef AR_SIMD
	m_data = _mm_div_ps(m_data, _mm_set1_ps(scalar));
#else
	m_comps[0] /= scalar;
	m_comps[1] /= scalar;
	m_comps[2] /= scalar;
	m_comps[3] /= scalar;
#endif // ifdef AR_SIMD

	return *this;
}

float32& AR_VEC_CALL Vector4::operator[](int32 i)
{
	AR_ASSERT_MSG(i >= 0 && i <= 3, "Index is out of range");
	return m_comps[i];
}

const float32& AR_VEC_CALL Vector4::operator[](int32 i) const
{
	AR_ASSERT_MSG(i >= 0 && i <= 3, "Index is out of range");
	return m_comps[i];
}

bool AR_VEC_CALL Vector4::operator==(const Vector4 &other) const
{
#ifdef AR_SIMD
	return 0xF == _mm_movemask_ps(_mm_cmp_ps(m_data, other.m_data, _CMP_EQ_OQ));
#else
	return (m_comps[0] == other.m_comps[0]) && (m_comps[1] == other.m_comps[1])
		&& (m_comps[2] == other.m_comps[2]) && (m_comps[3] == other.m_comps[3]);
#endif // ifdef AR_SIMD
}

bool AR_VEC_CALL Vector4::operator!=(const Vector4 &other) const
{
	return !(*this == other);
}

Vector4 AR_VEC_CALL operator*(float32 scalar, const Vector4 &v)
{
#ifdef AR_SIMD
	return Vector4(_mm_mul_ps(v.get128(), _mm_set1_ps(scalar)));
#else
	return Vector4(v.getX() * scalar, v.getY() * scalar,
		v.getZ() * scalar, v.getZ() * scalar);
#endif // ifdef AR_SIMD
}
} // namespace Argon
