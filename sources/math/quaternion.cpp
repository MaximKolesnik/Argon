#include <cmath>

#include <fundamental/debug.hpp>

#include "constants.hpp"
#include "quaternion.hpp"
#include "utils.hpp"

namespace Argon
{
Quaternion::Quaternion()
{
	Set(1.f, 0.f, 0.f, 0.f);
}

Quaternion::Quaternion(const Quaternion &) = default;

AR_VEC_CALL Quaternion::Quaternion(float32 q0, float32 q1, float32 q2, float32 q3)
{
	Set(q0, q1, q2, q3);
}

AR_VEC_CALL Quaternion::Quaternion(const Vector3 &axis, float32 angleRad)
{
	Set(axis, angleRad);
}

AR_VEC_CALL Quaternion::Quaternion(const Matrix3 &m)
{
	const float32 trace = m[0][0] + m[1][1] + m[2][2];
	Quaternion q;

	if (trace > 0)
	{
		const float32 s = std::sqrt(trace + 1.f) * 2.f;
		Set(
			0.25f * s,
			m[2][1] - m[1][2] / s,
			m[0][2] - m[2][0] / s,
			m[1][0] - m[0][1] / s);
	}
	else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2]))
	{
		const float32 s = std::sqrt(1.f + m[0][0] - m[1][1] - m[2][2]) * 2.f;
		Set(
			m[2][1] - m[1][2] / s,
			0.25f * s,
			(m[0][1] + m[1][0]) / s,
			(m[0][2] + m[2][0]) / s);
	}
	else if (m[1][1] > m[2][2])
	{
		const float32 s = std::sqrt(1.f + m[1][1] - m[0][0] - m[2][2]) * 2.f;
		Set(
			(m[0][2] - m[2][0]) / s,
			(m[0][1] + m[1][0]) / s,
			0.25f * s,
			(m[1][2] + m[2][1]) / s);
	}
	else
	{
		const float32 s = std::sqrt(1.f + m[2][2] - m[0][0] - m[1][1]) * 2.f;
		Set(
			(m[1][0] - m[0][1]) / s,
			(m[0][2] + m[2][0]) / s,
			(m[1][2] + m[2][1]) / s,
			0.25f * s);
	}
}

AR_VEC_CALL Quaternion::Quaternion(const Matrix4 &m)
{
	const float32 trace = m[0][0] + m[1][1] + m[2][2];
	Quaternion q;

	if (trace > 0)
	{
		const float32 s = std::sqrt(trace + 1.f) * 2.f;
		Set(
			0.25f * s,
			(m[2][1] - m[1][2]) / s,
			(m[0][2] - m[2][0]) / s,
			(m[1][0] - m[0][1]) / s);
	}
	else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2]))
	{
		const float32 s = std::sqrt(1.f + m[0][0] - m[1][1] - m[2][2]) * 2.f;
		Set(
			(m[2][1] - m[1][2]) / s,
			0.25f * s,
			(m[0][1] + m[1][0]) / s,
			(m[0][2] + m[2][0]) / s);
	}
	else if (m[1][1] > m[2][2])
	{
		const float32 s = std::sqrt(1.f + m[1][1] - m[0][0] - m[2][2]) * 2.f;
		Set(
			(m[0][2] - m[2][0]) / s,
			(m[0][1] + m[1][0]) / s,
			0.25f * s,
			(m[1][2] + m[2][1]) / s);
	}
	else
	{
		const float32 s = std::sqrt(1.f + m[2][2] - m[0][0] - m[1][1]) * 2.f;
		Set(
			(m[1][0] - m[0][1]) / s,
			(m[0][2] + m[2][0]) / s,
			(m[1][2] + m[2][1]) / s,
			0.25f * s);
	}
}

#ifdef AR_SIMD
	AR_VEC_CALL Quaternion::Quaternion(__m128 vec)
		: m_data(vec)
	{
	}

	__m128 AR_VEC_CALL Quaternion::Get128() const
	{
		return m_data;
	}
#endif // ifdef AR_SIMD

Matrix3 AR_VEC_CALL Quaternion::GetMatrix3() const
{
	return Matrix3(*this);
}

Matrix4 AR_VEC_CALL Quaternion::GetMatrix4() const
{
	return Matrix4(*this);
}

void AR_VEC_CALL Quaternion::GetEulerXYZ(float32 &roll, float32 &pitch,
																				 float32 &yaw) const
{
	const float32 test = m_comps[1] * m_comps[2] + m_comps[3] * m_comps[0];
	if (test > 0.499f)
	{
		yaw = 2.f * std::atan2(m_comps[1], m_comps[0]);
		pitch = c_pi / 2.f;
		roll = 0.f;

		return;
	}

	if (test < -0.499f)
	{
		yaw = -2.f * std::atan2(m_comps[1], m_comps[0]);
		pitch = -c_pi / 2.f;
		roll = 0.f;

		return;
	}

	const float32 sqx = m_comps[1] * m_comps[1];
	const float32 sqy = m_comps[2] * m_comps[2];
	const float32 sqz = m_comps[3] * m_comps[3];
	yaw = std::atan2(2 * m_comps[2] * m_comps[0] - 2 * m_comps[1] * m_comps[3],
		1 - 2 * sqy - 2 * sqz);

	pitch = std::asin(2 * test);

	roll = std::atan2(2 * m_comps[1] * m_comps[0] - 2 * m_comps[2] * m_comps[3],
		1 - 2 * sqx - 2 * sqz);
}

void AR_VEC_CALL Quaternion::GetAxisAngle(Vector3 &axis, float32 &angle) const
{
	//0 angle rotation, pick any axis
	if (m_comps[0] == 1.f)
	{
		angle = 0.f;
		axis = Vector3::c_xAxis;
		return;
	}
	//180 angle rotation
	else if (m_comps[0] == 0.f)
	{
		angle = c_pi;
		axis[0] = m_comps[1];
		axis[1] = m_comps[2];
		axis[2] = m_comps[3];
		return;
	}

	//No singularities
#ifdef AR_SIMD
	__m128 s = _mm_set1_ps(m_comps[0]);
	s = _mm_mul_ps(s, s);
	s = _mm_sub_ps(_mm_set1_ps(1), s);
	s = _mm_rsqrt_ps(s);

	__m128 vec = _mm_set_ps(0.0, m_comps[3], m_comps[2], m_comps[1]);
	vec = _mm_mul_ps(s, vec);

	__m128 mAngle = _mm_set1_ps(std::acos(m_comps[0]));
	mAngle = _mm_mul_ps(mAngle, _mm_set1_ps(2));

	angle = _mm_cvtss_f32(mAngle);
	axis.Set(vec);
#else
	angle = 2.f * std::cos(m_comps[0]);
	const float32 s =1.f / std::sqrt(1.f - m_comps[0] * m_comps[0]);

	axis[0] = m_comps[1] * s;
	axis[1] = m_comps[2] * s;
	axis[2] = m_comps[3] * s;
#endif // ifdef AR_SIMD
}

void AR_VEC_CALL Quaternion::Set(const Vector3 &axis, float32 angleRad)
{
	const float32 axisLength = axis.Length();

	AR_ASSERT_MSG(axisLength != 0.f, "Axis has length of 0");

	const float32 scalar = std::sin(angleRad * 0.5f) / axisLength;

	Set(std::cos(angleRad * 0.5f), axis.GetX() * scalar, axis.GetY() * scalar,
			axis.GetZ() * scalar);
}

void AR_VEC_CALL Quaternion::Set(const Matrix3 &m)
{
	const float32 trace = m[0][0] + m[1][1] + m[2][2];
	Quaternion q;

	if (trace > 0)
	{
		const float32 s = std::sqrt(trace + 1.f) * 2.f;
		Set(
			0.25f * s,
			(m[2][1] - m[1][2]) / s,
			(m[0][2] - m[2][0]) / s,
			(m[1][0] - m[0][1]) / s);
	}
	else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2]))
	{
		const float32 s = std::sqrt(1.f + m[0][0] - m[1][1] - m[2][2]) * 2.f;
		Set(
			(m[2][1] - m[1][2]) / s,
			0.25f * s,
			(m[0][1] + m[1][0]) / s,
			(m[0][2] + m[2][0]) / s);
	}
	else if (m[1][1] > m[2][2])
	{
		const float32 s = std::sqrt(1.f + m[1][1] - m[0][0] - m[2][2]) * 2.f;
		Set(
			(m[0][2] - m[2][0]) / s,
			(m[0][1] + m[1][0]) / s,
			0.25f * s,
			(m[1][2] + m[2][1]) / s);
	}
	else
	{
		const float32 s = std::sqrt(1.f + m[2][2] - m[0][0] - m[1][1]) * 2.f;
		Set(
			(m[1][0] - m[0][1]) / s,
			(m[0][2] + m[2][0]) / s,
			(m[1][2] + m[2][1]) / s,
			0.25f * s);
	}
}

void AR_VEC_CALL Quaternion::Set(const Matrix4 &m)
{
	const float32 trace = m[0][0] + m[1][1] + m[2][2];
	Quaternion q;

	if (trace > 0)
	{
		const float32 s = std::sqrt(trace + 1.f) * 2.f;
		Set(
			0.25f * s,
			(m[2][1] - m[1][2]) / s,
			(m[0][2] - m[2][0]) / s,
			(m[1][0] - m[0][1]) / s);
	}
	else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2]))
	{
		const float32 s = std::sqrt(1.f + m[0][0] - m[1][1] - m[2][2]) * 2.f;
		Set(
			(m[2][1] - m[1][2]) / s,
			0.25f * s,
			(m[0][1] + m[1][0]) / s,
			(m[0][2] + m[2][0]) / s);
	}
	else if (m[1][1] > m[2][2])
	{
		const float32 s = std::sqrt(1.f + m[1][1] - m[0][0] - m[2][2]) * 2.f;
		Set(
			(m[0][2] - m[2][0]) / s,
			(m[0][1] + m[1][0]) / s,
			0.25f * s,
			(m[1][2] + m[2][1]) / s);
	}
	else
	{
		const float32 s = std::sqrt(1.f + m[2][2] - m[0][0] - m[1][1]) * 2.f;
		Set(
			(m[1][0] - m[0][1]) / s,
			(m[0][2] + m[2][0]) / s,
			(m[1][2] + m[2][1]) / s,
			0.25f * s);
	}
}

void AR_VEC_CALL Quaternion::Set(float32 q0, float32 q1, float32 q2, float32 q3)
{
#ifdef AR_SIMD
	m_data = _mm_set_ps(q3, q2, q1, q0);
#else
	m_comps[0] = q0, m_comps[1] = q1, m_comps[2] = q2, m_comps[3] = q3;
#endif // ifdef AR_SIMD
}

void AR_VEC_CALL Quaternion::SetEuler(float32 roll, float32 pitch, float32 yaw)
{
	const float32 cx = std::cos(yaw * 0.5f);  //heading
	const float32 cy = std::cos(pitch * 0.5f);//attitude
	const float32 cz = std::cos(roll * 0.5f); //bank
	const float32 sx = std::sin(yaw * 0.5f);
	const float32 sy = std::sin(pitch * 0.5f);
	const float32 sz = std::sin(roll * 0.5f);

#ifdef AR_SIMD
	__m128 q1 = _mm_set1_ps(cz);
	__m128 q2 = _mm_set1_ps(sz);

	q2 = _mm_xor_ps(q2, _mm_castsi128_ps(MathUtils::c_SIMDNegMask));

	__m128 s = _mm_set_ps(sy, cy, sy, cy);
	q1 = _mm_mul_ps(q1, s);

	s = _mm_shuffle_ps(s, s, 0x11); //_MM_SHUFFLE(0, 1, 0, 1)
	q2 = _mm_mul_ps(q2, s);

	s = _mm_set_ps(cx, sx, sx, cx);
	q1 = _mm_mul_ps(q1, s);

	s = _mm_shuffle_ps(s, s, 0x41); //_MM_SHUFFLE(1, 0, 0, 1)
	q2 = _mm_mul_ps(q2, s);

	m_data = _mm_add_ps(q1, q2);
#else
	m_comps[0] = cx * cy * cz - sx * sy * sz;
	m_comps[1] = sx * sy * cz + cx * cy * sz;
	m_comps[2] = sx * cy * cz + cx * sy * sz;
	m_comps[3] = cx * sy * cz - sx * cy * sz;
#endif // ifdef AR_SIMD
}

Quaternion& AR_VEC_CALL Quaternion::Invert(void)
{
#ifdef AR_SIMD
	m_data = _mm_xor_ps(m_data, _mm_set_ps(-0.0f, -0.0f, -0.0f, +0.0f));
#else
	m_comps[1] = -m_comps[1], m_comps[2] = -m_comps[2], m_comps[3] = -m_comps[3];
#endif // ifdef AR_SIMD

	return *this;
}

Quaternion AR_VEC_CALL Quaternion::Inverted(void) const
{
#ifdef AR_SIMD
	return Quaternion(_mm_xor_ps(m_data, _mm_set_ps(-0.0f, -0.0f, -0.0f, +0.0f)));
#else
	return Quaternion(m_comps[0], -m_comps[1], -m_comps[2], -m_comps[3]);
#endif // ifdef AR_SIMD
}

float32 AR_VEC_CALL Quaternion::Dot(const Quaternion &other) const
{
#ifdef AR_SIMD
	const __m128 mul = _mm_mul_ps(m_data, other.m_data);
	//_MM_SHUFFLE(0, 0, 3, 2)
	const __m128 sum1 = _mm_add_ps(mul, _mm_shuffle_ps(mul, mul, 0xE));
	//_MM_SHUFFLE(0, 0, 0, 1)
	const __m128 sum = _mm_add_ps(sum1, _mm_shuffle_ps(sum1, sum1, 0x1));

	return _mm_cvtss_f32(sum);
#else
	return m_comps[0] * other.m_comps[0] + m_comps[1] * other.m_comps[1]
		+ m_comps[2] * other.m_comps[2] + m_comps[3] * other.m_comps[3];
#endif // ifdef AR_SIMD
}

float32 AR_VEC_CALL Quaternion::Length(void) const
{
#ifdef AR_SIMD
	const __m128 qMul = _mm_mul_ps(m_data, m_data);
	//_MM_SHUFFLE(0, 0, 3, 2)
	const __m128 sum1 = _mm_add_ps(qMul, _mm_shuffle_ps(qMul, qMul, 0xE));
	//_MM_SHUFFLE(0, 0, 0, 1)
	__m128 sum = _mm_add_ps(sum1, _mm_shuffle_ps(sum1, sum1, 0x1));

	sum = _mm_sqrt_ps(sum);

	return _mm_cvtss_f32(sum);
#else
	return std::sqrt(m_comps[0] * m_comps[0] + m_comps[1] * m_comps[1]
	+ m_comps[2] * m_comps[2] + m_comps[3] * m_comps[3]);
#endif // ifdef AR_SIMD
}

float32 AR_VEC_CALL Quaternion::LengthSq(void) const
{
#ifdef AR_SIMD
	const __m128 qMul = _mm_mul_ps(m_data, m_data);
	//_MM_SHUFFLE(0, 0, 3, 2)
	const __m128 sum1 = _mm_add_ps(qMul, _mm_shuffle_ps(qMul, qMul, 0xE));
	//_MM_SHUFFLE(0, 0, 0, 1)
	const __m128 sum = _mm_add_ps(sum1, _mm_shuffle_ps(sum1, sum1, 0x1));

	return _mm_cvtss_f32(sum);
#else
	return m_comps[0] * m_comps[0] + m_comps[1] * m_comps[1]
		+ m_comps[2] * m_comps[2] + m_comps[3] * m_comps[3];
#endif // ifdef AR_SIMD
}

Quaternion AR_VEC_CALL Quaternion::Normalized(void) const
{
	AR_ASSERT_MSG(this->Length() != 0.f, "Normalization of zero length Quaternion");

#ifdef AR_SIMD
	const __m128 qMul = _mm_mul_ps(m_data, m_data);
	__m128 length = _mm_add_ps(qMul,
		_mm_shuffle_ps(qMul, qMul, 0x1B)); //_MM_SHUFFLE(0, 1, 2, 3)
	length = _mm_add_ps(length,
		_mm_shuffle_ps(qMul, qMul, 0x7E)); //_MM_SHUFFLE(1, 3, 3, 2)
	length = _mm_add_ps(length,
		_mm_shuffle_ps(qMul, qMul, 0x81)); //_MM_SHUFFLE(2, 0, 0, 1)

	length = _mm_sqrt_ps(length);
	length = _mm_rcp_ps(length);

	return Quaternion(_mm_mul_ps(length, m_data));
#else
	const float32 length = 1.f / std::sqrt(m_comps[0] * m_comps[0]
		+ m_comps[1] * m_comps[1] + m_comps[2] * m_comps[2]
		+ m_comps[3] * m_comps[3]);

	return Quaternion(m_comps[0] * length, m_comps[1] * length,
		m_comps[2] * length, m_comps[3] * length);
#endif // ifdef AR_SIMD
}

Quaternion& AR_VEC_CALL Quaternion::Normalize(void)
{
	AR_ASSERT_MSG(this->Length() != 0.f, "Normalization of zero length Quaternion");

#ifdef AR_SIMD
	const __m128 qMul = _mm_mul_ps(m_data, m_data);
	__m128 length = _mm_add_ps(qMul,
		_mm_shuffle_ps(qMul, qMul, 0x1B)); //_MM_SHUFFLE(0, 1, 2, 3)
	length = _mm_add_ps(length,
		_mm_shuffle_ps(qMul, qMul, 0x7E)); //_MM_SHUFFLE(1, 3, 3, 2)
	length = _mm_add_ps(length,
		_mm_shuffle_ps(qMul, qMul, 0x81)); //_MM_SHUFFLE(2, 0, 0, 1)

	length = _mm_sqrt_ps(length);
	length = _mm_rcp_ps(length);

	m_data = _mm_mul_ps(length, m_data);
#else
	const float32 length = 1.f / std::sqrt(m_comps[0] * m_comps[0]
		+ m_comps[1] * m_comps[1] + m_comps[2] * m_comps[2]
		+ m_comps[3] * m_comps[3]);

	m_comps[0] *= length, m_comps[1] *= length,
		m_comps[2] *= length, m_comps[3] *= length;
#endif // ifdef AR_SIMD

	return *this;
}

Quaternion& AR_VEC_CALL Quaternion::operator=(const Quaternion &other)
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

Quaternion AR_VEC_CALL Quaternion::operator+(const Quaternion &other) const
{
#ifdef AR_SIMD
	return Quaternion(_mm_add_ps(m_data, other.m_data));
#else
	return Quaternion(m_comps[0] + other.m_comps[0], m_comps[1] + other.m_comps[1],
			m_comps[2] + other.m_comps[2], m_comps[3] + other.m_comps[3]);
#endif // ifdef AR_SIMD
}

Quaternion& AR_VEC_CALL Quaternion::operator+=(const Quaternion &other)
{
#ifdef AR_SIMD
	m_data = _mm_add_ps(m_data, other.m_data);
#else
	m_comps[0] += other.m_comps[0], m_comps[1] += other.m_comps[1],
			m_comps[2] += other.m_comps[2], m_comps[3] += other.m_comps[3];
#endif // ifdef AR_SIMD

	return *this;
}

Quaternion AR_VEC_CALL Quaternion::operator-(const Quaternion &other) const
{
#ifdef AR_SIMD
	return Quaternion(_mm_sub_ps(m_data, other.m_data));
#else
	return Quaternion(m_comps[0] - other.m_comps[0], m_comps[1] - other.m_comps[1],
			m_comps[2] - other.m_comps[2], m_comps[3] - other.m_comps[3]);
#endif // ifdef AR_SIMD
}

Quaternion& AR_VEC_CALL Quaternion::operator-=(const Quaternion &other)
{
#ifdef AR_SIMD
	m_data = _mm_sub_ps(m_data, other.m_data);
#else
	m_comps[0] -= other.m_comps[0], m_comps[1] -= other.m_comps[1],
			m_comps[2] -= other.m_comps[2], m_comps[3] -= other.m_comps[3];
#endif // ifdef AR_SIMD

	return *this;
}

Quaternion AR_VEC_CALL Quaternion::operator*(float32 scalar) const
{
#ifdef AR_SIMD
	__m128 s = _mm_load_ss(&scalar);
	s = _mm_shuffle_ps(s, s, 0x0);

	return Quaternion(_mm_mul_ps(m_data, s));
#else
	return Quaternion(m_comps[0] * scalar, m_comps[1] * scalar, m_comps[2] * scalar,
			m_comps[3] * scalar);
#endif // ifdef AR_SIMD
}

Quaternion& AR_VEC_CALL Quaternion::operator*=(float32 scalar)
{
#ifdef AR_SIMD
	const __m128 s = _mm_load_ss(&scalar);
	m_data = _mm_mul_ps(m_data, _mm_shuffle_ps(s, s, 0x0));
#else
	m_comps[0] *= scalar, m_comps[1] *= scalar, m_comps[2] *= scalar, m_comps[3] *= scalar;
#endif // ifdef AR_SIMD

	return *this;
}

Quaternion AR_VEC_CALL Quaternion::operator/(float32 scalar) const
{
#ifdef AR_SIMD
	__m128 s = _mm_load_ss(&scalar);
	s = _mm_shuffle_ps(s, s, 0x0);

	return Quaternion(_mm_div_ps(m_data, s));
#else
	return Quaternion(m_comps[0] / scalar, m_comps[1] / scalar,
			m_comps[2] / scalar, m_comps[3] / scalar);
#endif // ifdef AR_SIMD
}

Quaternion& AR_VEC_CALL Quaternion::operator/=(float32 scalar)
{
#ifdef AR_SIMD
	const __m128 s = _mm_load_ss(&scalar);
	m_data = _mm_mul_ps(m_data, _mm_shuffle_ps(s, s, 0x0));
#else
	m_comps[0] /= scalar, m_comps[1] /= scalar, m_comps[2] /= scalar,
			m_comps[3] /= scalar;
#endif // ifdef AR_SIMD

	return *this;
}

Quaternion AR_VEC_CALL Quaternion::operator*(const Quaternion &other) const
{
#ifdef AR_SIMD
	__m128 l = _mm_shuffle_ps(m_data, m_data, 0x1); //_MM_SHUFFLE(0, 0, 0, 1)
	__m128 r = _mm_shuffle_ps(other.m_data, other.m_data, 0xE5); //_MM_SHUFFLE(3, 2, 1, 1)

	__m128 res = _mm_mul_ps(l, r);

	l = _mm_shuffle_ps(m_data, m_data, 0x66); //_MM_SHUFFLE(1, 2, 1, 2)
	r = _mm_shuffle_ps(other.m_data, other.m_data, 0x82); //_MM_SHUFFLE(2, 0, 0, 2)

	res = _mm_add_ps(res, _mm_mul_ps(l, r));

	l = _mm_shuffle_ps(m_data, m_data, 0xFB); //_MM_SHUFFLE(3, 3, 2, 3)
	r = _mm_shuffle_ps(other.m_data, other.m_data, 0x1F); //_MM_SHUFFLE(0, 1, 3, 3)

	res = _mm_add_ps(res, _mm_mul_ps(l, r));

	l = _mm_shuffle_ps(m_data, m_data, 0x9C); //_MM_SHUFFLE(2, 1, 3, 0)
	r = _mm_shuffle_ps(other.m_data, other.m_data, 0x78); //_MM_SHUFFLE(1, 3, 2, 0)

	res = _mm_xor_ps(res, _mm_set_ps(+0.0f, +0.0f, +0.0f, -0.0f));
	l = _mm_mul_ps(l, r);
	l = _mm_xor_ps(l, _mm_set_ps(-0.0f, -0.0f, -0.0f, +0.0f));

	return Quaternion(_mm_add_ps(res, l));
#else
	return Quaternion(
		m_comps[0] * other.m_comps[0] - m_comps[1] * other.m_comps[1]
	 -m_comps[2] * other.m_comps[2] - m_comps[3] * other.m_comps[3],

		m_comps[0] * other.m_comps[1] + m_comps[1] * other.m_comps[0] +
		m_comps[2] * other.m_comps[3] - m_comps[3] * other.m_comps[2],

		m_comps[0] * other.m_comps[2] - m_comps[1] * other.m_comps[3] +
		m_comps[2] * other.m_comps[0] + m_comps[3] * other.m_comps[1],

		m_comps[0] * other.m_comps[3] + m_comps[1] * other.m_comps[2] -
		m_comps[2] * other.m_comps[1] + m_comps[3] * other.m_comps[0]);
#endif // ifdef AR_SIMD
}

Quaternion& AR_VEC_CALL Quaternion::operator*=(const Quaternion &other)
{
#ifdef AR_SIMD
	__m128 l = _mm_shuffle_ps(m_data, m_data, 0x1); //_MM_SHUFFLE(0, 0, 0, 1)
	__m128 r = _mm_shuffle_ps(other.m_data, other.m_data, 0xE5); //_MM_SHUFFLE(3, 2, 1, 1)

	__m128 res = _mm_mul_ps(l, r);

	l = _mm_shuffle_ps(m_data, m_data, 0x66); //_MM_SHUFFLE(1, 2, 1, 2)
	r = _mm_shuffle_ps(other.m_data, other.m_data, 0x82); //_MM_SHUFFLE(2, 0, 0, 2)

	res = _mm_add_ps(res, _mm_mul_ps(l, r));

	l = _mm_shuffle_ps(m_data, m_data, 0xFB); //_MM_SHUFFLE(3, 3, 2, 3)
	r = _mm_shuffle_ps(other.m_data, other.m_data, 0x1F); //_MM_SHUFFLE(0, 1, 3, 3)

	res = _mm_add_ps(res, _mm_mul_ps(l, r));

	l = _mm_shuffle_ps(m_data, m_data, 0x9C); //_MM_SHUFFLE(2, 1, 3, 0)
	r = _mm_shuffle_ps(other.m_data, other.m_data, 0x78); //_MM_SHUFFLE(1, 3, 2, 0)

	res = _mm_xor_ps(res, _mm_set_ps(+0.0f, +0.0f, +0.0f, -0.0f));
	l = _mm_mul_ps(l, r);
	l = _mm_xor_ps(l, _mm_set_ps(-0.0f, -0.0f, -0.0f, +0.0f));

	m_data = _mm_add_ps(res, l);
#else
	Set(m_comps[0] * other.m_comps[0] - m_comps[1] * other.m_comps[1]
			- m_comps[2] * other.m_comps[2] - m_comps[3] * other.m_comps[3],
			m_comps[0] * other.m_comps[1] + m_comps[1] * other.m_comps[0] +
			m_comps[2] * other.m_comps[3] - m_comps[3] * other.m_comps[2],
			m_comps[0] * other.m_comps[2] - m_comps[1] * other.m_comps[3] +
			m_comps[2] * other.m_comps[0] + m_comps[3] * other.m_comps[1],
			m_comps[0] * other.m_comps[3] + m_comps[1] * other.m_comps[2] -
			m_comps[2] * other.m_comps[1] + m_comps[3] * other.m_comps[0]);
#endif // ifdef AR_SIMD

	return *this;
}

Quaternion AR_VEC_CALL Quaternion::operator*(const Vector3 &v) const
{
#ifdef AR_SIMD
	__m128 q = _mm_shuffle_ps(m_data, m_data, 0x1); //_MM_SHUFFLE(0, 0, 0, 1)
	__m128 mv = _mm_shuffle_ps(v.Get128(), v.Get128(), 0x90); //_MM_SHUFFLE(2, 1, 0, 0)

	__m128 result = _mm_mul_ps(q, mv);

	q = _mm_shuffle_ps(m_data, m_data, 0x7A); //_MM_SHUFFLE(1, 3, 2, 2)
	mv = _mm_shuffle_ps(v.Get128(), v.Get128(), 0x49); //_MM_SHUFFLE(1, 0, 2, 1)

	result = _mm_add_ps(result, _mm_mul_ps(q, mv));

	q = _mm_shuffle_ps(m_data, m_data, 0x9F); //_MM_SHUFFLE(2, 1, 3, 3)
	mv = _mm_shuffle_ps(v.Get128(), v.Get128(), 0x26); //_MM_SHUFFLE(0, 2, 1, 2)

	q = _mm_mul_ps(q, mv);


	result = _mm_xor_ps(result, _mm_castsi128_ps(MathUtils::c_SIMDNegMask));
	q = _mm_xor_ps(q, MathUtils::c_SIMDNegMask);

	return Quaternion( _mm_add_ps(result, q));
#else
	return Quaternion(-m_comps[1] * v[0] - m_comps[2] * v[1] - m_comps[3] * v[2],
										 m_comps[0] * v[0] + m_comps[2] * v[2] - m_comps[3] * v[1],
										 m_comps[0] * v[1] + m_comps[3] * v[0] - m_comps[1] * v[2],
										 m_comps[0] * v[2] + m_comps[1] * v[1] - m_comps[2] * v[0]);
#endif // ifdef AR_SIMD
}

Quaternion AR_VEC_CALL Quaternion::Slerp(const Quaternion &q, float32 t) const
{
	Quaternion slerped;

	const float32 cosHalfTheta = m_comps[0] * q[0] + m_comps[1] * q[1]
		+ m_comps[2] * q[2] + m_comps[3] * q[3];

	if (std::abs(cosHalfTheta) >= 1.f)
	{
		slerped[0] = m_comps[0];
		slerped[1] = m_comps[1];
		slerped[2] = m_comps[2];
		slerped[3] = m_comps[3];
		return slerped;
	}

	const float32 halfTheta = std::cos(cosHalfTheta);
	const float32 sinHalfTheta = std::sqrt(1.f - cosHalfTheta * cosHalfTheta);

	if (std::abs(sinHalfTheta) < 0.001f)
	{
		slerped.Set(
					0.5f * (m_comps[0] + q[0]),
					0.5f * (m_comps[1] + q[1]),
					0.5f * (m_comps[2] + q[2]),
					0.5f * (m_comps[3] + q[3]));

		return slerped;
	}

	const float32 ratioA = std::sin((1 - t) * halfTheta) / sinHalfTheta;
	const float32 ratioB = std::sin(t * halfTheta) / sinHalfTheta;

	return *this * ratioA + q * ratioB;
}

Vector3 AR_VEC_CALL Quaternion::Rotated(const Vector3 &v) const
{
	const Quaternion q = *this * v * Inverted();

	return Vector3(q[1], q[2], q[3]);
}

Vector3& AR_VEC_CALL Quaternion::Rotate(Vector3 &v) const
{
	const Quaternion q = *this * v * Inverted();

	v.Set(q[1], q[2], q[3]);

	return v;
}

float32& AR_VEC_CALL Quaternion::operator[](int32 i)
{
	AR_ASSERT_MSG(i >= 0 && i < 4, "Index is out of range");
	return m_comps[i];
}

const float32& AR_VEC_CALL Quaternion::operator[](int32 i) const
{
	AR_ASSERT_MSG(i >= 0 && i < 4, "Index is out of range");
	return m_comps[i];
}
} // namespace Argon
