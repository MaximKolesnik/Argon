#include <algorithm>
#include <cmath>
#include <limits>

#include <fundamental/debug.hpp>

#include "constants.hpp"
#include "utils.hpp"
#include "vector3.hpp"

namespace argon::math
{
const Vector3 AR_ATTR_ALIGN(16) Vector3::c_zero = Vector3(0.f, 0.f, 0.f);
const Vector3 AR_ATTR_ALIGN(16) Vector3::c_xAxis = Vector3(1.f, 0.f, 0.f);
const Vector3 AR_ATTR_ALIGN(16) Vector3::c_yAxis = Vector3(0.f, 1.f, 0.f);
const Vector3 AR_ATTR_ALIGN(16) Vector3::c_zAxis = Vector3(0.f, 0.f, 1.f);

Vector3::Vector3()
{

}

AR_VEC_CALL Vector3::Vector3(const Vector3 &) = default;

AR_VEC_CALL Vector3::Vector3(float32 x, float32 y, float32 z)
{
#ifdef AR_SIMD
	m_data = _mm_set_ps(0.0, z, y, x);
#else
	m_comps[0] = x, m_comps[1] = y, m_comps[2] = z, m_comps[3] = 0.f;
#endif // ifdef AR_SIMD
}

#ifdef AR_SIMD
AR_VEC_CALL Vector3::Vector3(__m128 data)
{
	m_data = data;
}

__m128 AR_VEC_CALL Vector3::get128(void) const
{
	return m_data;
}

void AR_VEC_CALL Vector3::set(__m128 data)
{
	m_data = data;
}
#endif // ifdef AR_SIMD

float32 AR_VEC_CALL Vector3::getX(void) const
{
	return m_comps[0];
}

float32 AR_VEC_CALL Vector3::getY(void) const
{
	return m_comps[1];
}

float32 AR_VEC_CALL Vector3::getZ(void) const
{
	return m_comps[2];
}

void AR_VEC_CALL Vector3::setX(float32 x)
{
	m_comps[0] = x;
}

void AR_VEC_CALL Vector3::setY(float32 y)
{
	m_comps[1] = y;
}

void AR_VEC_CALL Vector3::setZ(float32 z)
{
	m_comps[2] = z;
}

void AR_VEC_CALL Vector3::set(float32 x, float32 y, float32 z)
{
#ifdef AR_SIMD
	m_data = _mm_set_ps(0.0, z, y, x);
#else
	m_comps[0] = x, m_comps[1] = y, m_comps[2] = z;
#endif // ifdef AR_SIMD
}

void AR_VEC_CALL Vector3::splat(float32 xyz)
{
#ifdef AR_SIMD
	m_data = _mm_set1_ps(xyz);
	m_comps[3] = 0.f;
#else
	m_comps[0] = m_comps[1] = m_comps[2] = xyz;
#endif // ifdef AR_SIMD
}

float32 AR_VEC_CALL Vector3::dot(const Vector3 &other) const
{
#ifdef AR_SIMD
	__m128 d = _mm_mul_ps(m_data, other.m_data);
	const __m128 z = _mm_shuffle_ps(d, d, 0xFE); //_MM_SHUFFLE(3, 3, 3, 2)
	const __m128 y = _mm_shuffle_ps(d, d, 0XFD); //_MM_SHUFFLE(3, 3, 3, 1)

	d = _mm_add_ps(d, y);
	d = _mm_add_ps(d, z);

	return _mm_cvtss_f32(d);
#else
	return m_comps[0] * other.m_comps[0] + m_comps[1] * other.m_comps[1]
		+ m_comps[2] * other.m_comps[2];
#endif // ifdef AR_SIMD
}

Vector3 AR_VEC_CALL Vector3::cross(const Vector3 &other) const
{
#ifdef AR_SIMD
	__m128 left = _mm_shuffle_ps(other.m_data, other.m_data, 0xC9); //_MM_SHUFFLE(3, 0, 2, 1)
	left = _mm_mul_ps(left, m_data);

	__m128 right = _mm_shuffle_ps(m_data, m_data, 0xC9); //_MM_SHUFFLE(3, 0, 2, 1)
	right = _mm_mul_ps(right, other.m_data);

	left = _mm_sub_ps(left, right);

	return Vector3(_mm_shuffle_ps(left, left, 0xC9));
#else
	return Vector3(m_comps[1] * other.m_comps[2] - m_comps[2] * other.m_comps[1],
		m_comps[2] * other.m_comps[0] - m_comps[0] * other.m_comps[2],
		m_comps[0] * other.m_comps[1] - m_comps[1] * other.m_comps[0]);
#endif // ifdef AR_SIMD
}

float32 AR_VEC_CALL Vector3::getLength(void) const
{
	return std::sqrt(this->dot(*this));
}

float32 AR_VEC_CALL Vector3::getLengthSq(void) const
{
	return this->dot(*this);
}

Vector3& AR_VEC_CALL Vector3::normalize(void)
{
	AR_ASSERT_MSG(!isZeroEpsilon(), "Trying to normalize vector of length 0");
	return *this /= getLength();
}

Vector3 AR_VEC_CALL Vector3::getNormalized(void) const
{
	AR_ASSERT_MSG(!isZeroEpsilon(), "Trying to normalize vector of length 0");
	return *this / getLength();
}

void Vector3::zeroOut()
{
#ifdef AR_SIMD
	m_data = _mm_setzero_ps();
#else
	m_comps[0] = m_comps[1] = m_comps[2] = m_comps[3] = 0.0;
#endif // ifdef AR_SIMD
}

bool Vector3::isZeroEpsilon() const
{
	 return getLengthSq() < c_epsilon * c_epsilon;
}

bool Vector3::hasInfinity() const
{
#ifdef AR_SIMD
	__m128 test = _mm_andnot_ps(utils::c_SIMDZeroMask, m_data);
	const __m128 inf = _mm_set1_ps(std::numeric_limits<float32>::infinity());

	test = _mm_cmp_ps(test, inf, _CMP_EQ_OQ);

	return _mm_movemask_ps(test) != 0;
#else
	const float32 inf = std::numeric_limits<float32>::infinity();

	return (m_comps[0] == inf) || (m_comps[1] == inf) || (m_comps[2] == inf);
#endif // ifdef AR_SIMD
}

Vector3 AR_VEC_CALL Vector3::getAbs(void) const
{
#ifdef AR_SIMD
	return Vector3(_mm_and_ps(m_data, utils::c_SIMDAbsMask));
#else
	return Vector3(std::abs(m_comps[0]), std::abs(m_comps[1]), std::abs(m_comps[2]));
#endif // ifdef AR_SIMD
}

Vector3& AR_VEC_CALL Vector3::abs(void)
{
#ifdef AR_SIMD
	m_data = _mm_and_ps(m_data, utils::c_SIMDAbsMask);
#else
	m_comps[0] = std::abs(m_comps[0]);
	m_comps[1] = std::abs(m_comps[1]);
	m_comps[2] = std::abs(m_comps[2]);
#endif // ifdef AR_SIMD

	return *this;
}

Vector3 AR_VEC_CALL Vector3::getNegated(void) const
{
#ifdef AR_SIMD
	return Vector3(_mm_xor_ps(m_data, utils::c_SIMDNegMask));
#else
	return Vector3(-m_comps[0], -m_comps[1], -m_comps[2]);
#endif // ifdef AR_SIMD
}

//Negates the current vector
Vector3& AR_VEC_CALL Vector3::negate(void)
{
#ifdef AR_SIMD
	m_data = _mm_xor_ps(m_data, utils::c_SIMDNegMask);
#else
	m_comps[0] = -m_comps[0];
	m_comps[1] = -m_comps[1];
	m_comps[2] = -m_comps[2];
#endif // ifdef AR_SIMD

	return *this;
}

//Return distance between two points
float32 AR_VEC_CALL Vector3::getDistance(const Vector3 &other) const
{
	return (*this - other).getLength();
}

float32 AR_VEC_CALL Vector3::getDistanceSq(const Vector3 &other) const
{
	return (*this - other).getLengthSq();
}

int32 AR_VEC_CALL Vector3::getMaxAxis(void) const
{
	return m_comps[0] < m_comps[1] ? (m_comps[1] < m_comps[2] ? 2 : 1)
																 : (m_comps[0] < m_comps[2] ? 2 : 0);
}

//Return the index of the min axis
int32 AR_VEC_CALL Vector3::getMinAxis(void) const
{
	return m_comps[0] < m_comps[1] ? (m_comps[0] < m_comps[2] ? 0 : 2)
																 : (m_comps[1] < m_comps[2] ? 1 : 2);
}

//Return the value of the max axis
float32 AR_VEC_CALL Vector3::getMaxAxisValue(void) const
{
	return std::max({m_comps[0], m_comps[1], m_comps[2]});
}

//Return the value of the min axis
float32 AR_VEC_CALL Vector3::getMinAxisValue(void) const
{
	return std::min({m_comps[0], m_comps[1], m_comps[2]});
}

//Linear interpolation
Vector3 AR_VEC_CALL Vector3::lerp(const Vector3 &other, float32 t) const
{
#ifdef AR_SIMD
	__m128 mt = _mm_load_ss(&t);
	mt = _mm_shuffle_ps(mt, mt, 0xC0); //_MM_SHUFFLE(3, 0, 0, 0);

	const __m128 lerpDir = _mm_sub_ps(other.m_data, m_data);
	const __m128 lerpMove = _mm_mul_ps(lerpDir, mt);

	return Vector3(_mm_add_ps(m_data, lerpMove));
#else
	return (*this) + (other - *this) * t;
#endif // ifdef AR_SIMD
}

Vector3 AR_VEC_CALL Vector3::getRotatedRad(const Vector3 &axis, float32 angleRad) const
{
#ifdef AR_SIMD
	const float32 sinVal = std::sin(angleRad);
	const float32 cosVal = std::cos(angleRad);

	__m128 mSin = _mm_load_ss(&sinVal);
	mSin = _mm_shuffle_ps(mSin, mSin, 0xC0); //_MM_SHUFFLE(3, 0, 0, 0)

	__m128 mCos = _mm_load_ss(&cosVal);
	mCos = _mm_shuffle_ps(mCos, mCos, 0xC0); //_MM_SHUFFLE(3, 0, 0, 0)

	__m128 p = _mm_mul_ps(m_data, axis.m_data);
	const __m128 py = _mm_shuffle_ps(p, p, 0xD2); //(z, x, y, 0) _MM_SHUFFLE(3, 1, 0, 2)
	const __m128 pz = _mm_shuffle_ps(p, p, 0xC9); //(y, z, x, 0) _MM_SHUFFLE(3, 0, 2, 1)

	p = _mm_add_ps(p, py);
	p = _mm_add_ps(p, pz);
	p = _mm_mul_ps(axis.m_data, p);

	__m128 n = axis.cross(*this).get128();

	__m128 d = _mm_sub_ps(m_data, p);

	n = _mm_mul_ps(n, mSin);
	d = _mm_mul_ps(d, mCos);

	p = _mm_add_ps(d, p);
	return Vector3(_mm_add_ps(p, n));
#else
	const Vector3 p = axis * this->dot(axis);
	const Vector3 n = axis.cross(*this);
	const Vector3 d = *this - p;

	return d * std::cos(angleRad) + n * std::sin(angleRad) + p;
#endif // ifdef AR_SIMD
}

Vector3& AR_VEC_CALL Vector3::rotateRad(const Vector3 &axis, float32 angleRad)
{
#ifdef AR_SIMD
	m_data = this->getRotatedRad(axis, angleRad).m_data;
#else
	*this = this->getRotatedRad(axis, angleRad);
#endif // ifdef AR_SIMD

	return *this;
}

Vector3 AR_VEC_CALL Vector3::getRotatedDeg(const Vector3 &axis, float32 angleDeg) const
{
	return this->getRotatedRad(axis, angleDeg * c_rads_per_deg);
}

Vector3& AR_VEC_CALL Vector3::rotateDeg(const Vector3 &axis, float32 angleDeg)
{
	return this->rotateRad(axis, angleDeg * c_rads_per_deg);
}

float32 AR_VEC_CALL Vector3::getAngle(const Vector3 &other) const
{
	const float32 scalar = std::sqrt(this->getLengthSq() * other.getLengthSq());

	AR_ASSERT_MSG(scalar != 0.f, "One of the vectors has length 0");

	return std::acos(this->dot(other) / scalar);
}

Vector3& AR_VEC_CALL Vector3::Vector3::operator=(const Vector3 &other)
{
#ifdef AR_SIMD
	m_data = other.m_data;
#else
	m_comps[0] = other.m_comps[0];
	m_comps[1] = other.m_comps[1];
	m_comps[2] = other.m_comps[2];
#endif // ifdef AR_SIMD

	return *this;
}

Vector3 AR_VEC_CALL Vector3::operator+(const Vector3 &other) const
{
#ifdef AR_SIMD
	return Vector3(_mm_add_ps(m_data, other.m_data));
#else
	return Vector3(m_comps[0] + other.m_comps[0], m_comps[1] + other.m_comps[1],
		m_comps[2] + other.m_comps[2]);
#endif // ifdef AR_SIMD
}

Vector3& AR_VEC_CALL Vector3::operator+=(const Vector3 &other)
{
#ifdef AR_SIMD
	m_data = _mm_add_ps(m_data, other.m_data);
#else
	m_comps[0] += other.m_comps[0];
	m_comps[1] += other.m_comps[1];
	m_comps[2] += other.m_comps[2];
#endif // ifdef AR_SIMD

	return *this;
}

Vector3 AR_VEC_CALL Vector3::operator-() const
{
#ifdef AR_SIMD
	return Vector3(_mm_xor_ps(m_data, utils::c_SIMDNegMask));
#else
	return Vector3(-m_comps[0], -m_comps[1], -m_comps[2]);
#endif // ifdef AR_SIMD
}

Vector3 AR_VEC_CALL Vector3::operator-(const Vector3 &other) const
{
#ifdef AR_SIMD
	return Vector3(_mm_sub_ps(m_data, other.m_data));
#else
	return Vector3(m_comps[0] - other.m_comps[0], m_comps[1] - other.m_comps[1],
		m_comps[2] - other.m_comps[2]);
#endif // ifdef AR_SIMD
}

Vector3& AR_VEC_CALL Vector3::operator-=(const Vector3 &other)
{
#ifdef AR_SIMD
	m_data = _mm_sub_ps(m_data, other.m_data);
#else
	m_comps[0] -= other.m_comps[0];
	m_comps[1] -= other.m_comps[1];
	m_comps[2] -= other.m_comps[2];
#endif // ifdef AR_SIMD

	return *this;
}

//Scaling operators
Vector3 AR_VEC_CALL Vector3::operator*(float32 scalar) const
{
#ifdef AR_SIMD
	return Vector3(_mm_mul_ps(m_data, _mm_set1_ps(scalar)));
#else
	return Vector3(m_comps[0] * scalar, m_comps[1] * scalar, m_comps[2] * scalar);
#endif // ifdef AR_SIMD
}

Vector3& AR_VEC_CALL Vector3::operator*=(float32 scalar)
{
#ifdef AR_SIMD
	m_data = _mm_mul_ps(m_data, _mm_set1_ps(scalar));
#else
	m_comps[0] *= scalar;
	m_comps[1] *= scalar;
	m_comps[2] *= scalar;
#endif // ifdef AR_SIMD

	return *this;
}

Vector3 AR_VEC_CALL Vector3::operator*(const Vector3 &other) const
{
#ifdef AR_SIMD
	return Vector3(_mm_mul_ps(m_data, other.get128()));
#else
	return Vector3(m_comps[0] * other.m_comps[0], m_comps[1] * other.m_comps[1],
		m_comps[2] * other.m_comps[2]);
#endif // ifdef AR_SIMD
}

Vector3 AR_VEC_CALL Vector3::operator/(const Vector3 &other) const
{
	AR_ASSERT_MSG(other.m_comps[0] != 0.f && other.m_comps[1] != 0.f
								&& other.m_comps[2] != 0.f, "Division by zero");
#ifdef AR_SIMD
	return Vector3(_mm_div_ps(m_data, other.get128()));
#else
	return Vector3(m_comps[0] / other.m_comps[0], m_comps[1] / other.m_comps[1],
		m_comps[2] / other.m_comps[2]);
#endif // ifdef AR_SIMD
}

Vector3 AR_VEC_CALL Vector3::operator/(float32 scalar) const
{
	AR_ASSERT_MSG(scalar != 0.f, "Scaling by 1/0");
#ifdef AR_SIMD
	return Vector3(_mm_div_ps(m_data, _mm_set1_ps(scalar)));
#else
	return Vector3(m_comps[0] / scalar, m_comps[1] / scalar,
		m_comps[2] / scalar);
#endif // ifdef AR_SIMD
}

Vector3& AR_VEC_CALL Vector3::operator/=(float32 scalar)
{
	AR_ASSERT_MSG(scalar != 0.f, "Scaling by 1/0");
#ifdef AR_SIMD
	m_data = _mm_div_ps(m_data, _mm_set1_ps(scalar));
#else
	m_comps[0] /= scalar;
	m_comps[1] /= scalar;
	m_comps[2] /= scalar;
#endif // ifdef AR_SIMD

	return *this;
}

float32& AR_VEC_CALL Vector3::operator[](int32 i)
{
	AR_ASSERT_MSG(i >= 0 && i <= 2, "Index is out of range");
	return m_comps[i];
}

const float32& AR_VEC_CALL Vector3::operator[](int32 i) const
{
	AR_ASSERT_MSG(i >= 0 && i <= 2, "Index is out of range");
	return m_comps[i];
}

bool AR_VEC_CALL Vector3::operator==(const Vector3 &other) const
{
#ifdef AR_SIMD
	return 0xF == _mm_movemask_ps(_mm_cmp_ps(m_data, other.m_data, _CMP_EQ_OQ));
#else
	return (m_comps[0] == other.m_comps[0]) && (m_comps[1] == other.m_comps[1])
		&& (m_comps[2] == other.m_comps[2]);
#endif // ifdef AR_SIMD
}

bool AR_VEC_CALL Vector3::operator!=(const Vector3 &other) const
{
	return !(*this == other);
}

void AR_VEC_CALL Vector3::round(uint16 numDecimals)
{
	m_comps[0] = utils::round(m_comps[0], numDecimals);
	m_comps[1] = utils::round(m_comps[1], numDecimals);
	m_comps[2] = utils::round(m_comps[2], numDecimals);
}

Vector3 AR_VEC_CALL Vector3::getRounded(uint16 numDecimals) const
{
	Vector3 rVec;

	rVec[0] = utils::round(m_comps[0], numDecimals);
	rVec[1] = utils::round(m_comps[1], numDecimals);
	rVec[2] = utils::round(m_comps[2], numDecimals);

	return rVec;
}

Vector3 AR_VEC_CALL operator*(float32 scalar, const Vector3 &v)
{
#ifdef AR_SIMD
	return Vector3(_mm_mul_ps(v.get128(), _mm_set1_ps(scalar)));
#else
	return Vector3(v.getX() * scalar, v.getY() * scalar,
		v.getZ() * scalar);
#endif // ifdef AR_SIMD
}
} // namespace argon::math
