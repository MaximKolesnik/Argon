#include <cmath>

#include <fundamental/debug.hpp>

#include "constants.hpp"
#include "matrix4.hpp"
#include "quaternion.hpp"

namespace Argon
{
const Matrix4 AR_ATTR_ALIGN(16) Matrix4::c_identity(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f);

Matrix4::Matrix4()
{
}

AR_VEC_CALL Matrix4::Matrix4(const Matrix4 &other)
{
	m_rows[0] = other.m_rows[0];
	m_rows[1] = other.m_rows[1];
	m_rows[2] = other.m_rows[2];
	m_rows[3] = other.m_rows[3];
}

AR_VEC_CALL Matrix4::Matrix4(const Quaternion &q)
{
	float32 s = q.Length();
	AR_ASSERT_MSG(s != 0.f, "Quaternion of length 0");
	s = 2.f / s;

#ifdef AR_SIMD
	__m128 row0, row1, row2;
	const __m128 qData = q.Get128();
	// Y Y W
	row0 = _mm_shuffle_ps(qData, qData, 0xA); //_MM_SHUFFLE(0, 0, 2, 2)
	// X X Z
	row1 = _mm_shuffle_ps(qData, qData, 0x35); //_MM_SHUFFLE(0, 3, 1, 1)
	// W W Y
	row2 = _mm_shuffle_ps(qData, qData, 0x20); //_MM_SHUFFLE(0, 2, 0, 0)

	// Y X Y Z
	__m128 temp = _mm_shuffle_ps(qData, qData, 0xE6); //_MM_SHUFFLE(3, 2, 1, 2)
	row0 = _mm_mul_ps(row0, temp);
	row1 = _mm_mul_ps(row1, temp);
	row2 = _mm_mul_ps(row2, temp);

	row0 = _mm_xor_ps(row0, _mm_set_ps(+0.f, +0.f, -0.f, +0.f));
	row1 = _mm_xor_ps(row1, _mm_set_ps(+0.f, -0.f, +0.f, +0.f));
	row2 = _mm_xor_ps(row2, _mm_set_ps(+0.f, +0.f, +0.f, -0.f));

	// Z Z X W
	temp = _mm_shuffle_ps(qData, qData, 0x1F); //_MM_SHUFFLE(0, 1, 3, 3)

	row0 = _mm_add_ps(row0,
		_mm_mul_ps(_mm_shuffle_ps(qData, qData, 0x33), temp)); //_MM_SHUFFLE(0, 3, 0, 3)
	row1 = _mm_add_ps(row1,
		_mm_mul_ps(_mm_shuffle_ps(qData, qData, 0x4C), temp)); //_MM_SHUFFLE(1, 0, 3, 0)
	row2 = _mm_add_ps(row2,
		_mm_mul_ps(_mm_shuffle_ps(qData, qData, 0x19), temp)); //_MM_SHUFFLE(0, 1, 2, 1)

	row0 = _mm_xor_ps(row0, _mm_set_ps(+0.f, +0.f, -0.f, +0.f));
	row1 = _mm_xor_ps(row1, _mm_set_ps(+0.f, -0.f, +0.f, +0.f));

	const __m128 scalar = _mm_set1_ps(s);
	row0 = _mm_mul_ps(scalar, row0);
	row1 = _mm_mul_ps(scalar, row1);
	row2 = _mm_mul_ps(scalar, row2);

	row0 = _mm_xor_ps(row0, _mm_set_ps(+0.f, +0.f, +0.f, -0.f));
	row1 = _mm_xor_ps(row1, _mm_set_ps(+0.f, +0.f, -0.f, +0.f));
	row2 = _mm_xor_ps(row2, _mm_set_ps(+0.f, -0.f, +0.f, +0.f));

	row0 = _mm_add_ps(_mm_set_ps(0.f, 0.f, 0.f, 1.f), row0);
	row1 = _mm_add_ps(_mm_set_ps(0.f, 0.f, 1.f, 0.f), row1);
	row2 = _mm_add_ps(_mm_set_ps(0.f, 1.f, 0.f, 0.f), row2);

	m_rows[0].Set(row0);
	m_rows[1].Set(row1);
	m_rows[2].Set(row2);
	m_rows[3].Set(0.f, 0.f, 0.f, 1.f);
	m_rows[0][3] = 0.f;
	m_rows[1][3] = 0.f;
	m_rows[2][3] = 0.f;
#else
	const float32 xy = s * q[1] * q[2];
	const float32 wz = s * q[0] * q[3];
	const float32 xz = s * q[1] * q[3];
	const float32 wy = s * q[0] * q[2];
	const float32 yz = s * q[2] * q[3];
	const float32 wx = s * q[0] * q[1];
	const float32 yy = s * q[2] * q[2];
	const float32 xx = s * q[1] * q[1];
	const float32 zz = s * q[3] * q[3];

	Set(
		1 - yy - zz, xy - wz, xz + wy, 0.f,
		xy + wz, 1 - xx - zz, yz - wx, 0.f,
		xz - wy, yz + wx, 1 - xx - yy, 0.f,
		0.f, 0.f, 0.f, 1.f
		);
#endif // ifdef AR_SIMD
}

AR_VEC_CALL Matrix4::Matrix4(
			float32 e00, float32 e01, float32 e02, float32 e03,
			float32 e10, float32 e11, float32 e12, float32 e13,
			float32 e20, float32 e21, float32 e22, float32 e23,
			float32 e30, float32 e31, float32 e32, float32 e33)
{
	m_rows[0].Set(e00, e01, e02, e03);
	m_rows[1].Set(e10, e11, e12, e13);
	m_rows[2].Set(e20, e21, e22, e23);
	m_rows[3].Set(e30, e31, e32, e33);
}

AR_VEC_CALL Matrix4::Matrix4(const Vector4 &v0, const Vector4 &v1,
														 const Vector4 &v2, const Vector4 &v3)
{
	m_rows[0] = v0;
	m_rows[1] = v1;
	m_rows[2] = v2;
	m_rows[3] = v3;
}

// Will set e33 to 1.0
AR_VEC_CALL Matrix4::Matrix4(const Vector3 &v0, const Vector3 &v1,
														 const Vector3 &v2, const Vector3 &v3)
{
	m_rows[0].Set(v0.GetX(), v1.GetX(), v2.GetX(), v3.GetX());
	m_rows[1].Set(v0.GetY(), v1.GetY(), v2.GetY(), v3.GetY());
	m_rows[2].Set(v0.GetZ(), v1.GetZ(), v2.GetZ(), v3.GetZ());
	m_rows[3].Set(0.f, 0.f, 0.f, 1.f);
}

#ifdef AR_SIMD
AR_VEC_CALL Matrix4::Matrix4(__m128 v0, __m128 v1, __m128 v2, __m128 v3)
	{
		m_rows[0] = Vector4(v0);
		m_rows[1] = Vector4(v1);
		m_rows[2] = Vector4(v2);
		m_rows[3] = Vector4(v3);
	}
#endif

Matrix4& AR_VEC_CALL Matrix4::operator=(const Matrix4 &other)
{
	m_rows[0] = other.m_rows[0];
	m_rows[1] = other.m_rows[1];
	m_rows[2] = other.m_rows[2];
	m_rows[3] = other.m_rows[3];

	return *this;
}

void AR_VEC_CALL Matrix4::Set(
			float32 e00, float32 e01, float32 e02, float32 e03,
			float32 e10, float32 e11, float32 e12, float32 e13,
			float32 e20, float32 e21, float32 e22, float32 e23,
			float32 e30, float32 e31, float32 e32, float32 e33)
{
	m_rows[0].Set(e00, e01, e02, e03);
	m_rows[1].Set(e10, e11, e12, e13);
	m_rows[2].Set(e20, e21, e22, e23);
	m_rows[3].Set(e30, e31, e32, e33);
}

void Matrix4::SetIdentity(void)
{
	m_rows[0].Set(1.f, 0.f, 0.f, 0.f);
	m_rows[1].Set(0.f, 1.f, 0.f, 0.f);
	m_rows[2].Set(0.f, 0.f, 1.f, 0.f);
	m_rows[3].Set(0.f, 0.f, 0.f, 1.f);
}

void Matrix4::ZeroOut(void)
{
	m_rows[0].ZeroOut();
	m_rows[1].ZeroOut();
	m_rows[2].ZeroOut();
	m_rows[3].ZeroOut();
}

void AR_VEC_CALL Matrix4::SetRotationRad(float32 roll, float32 pitch, float32 yaw)
{
	const float32 cx = std::cos(yaw);  //heading
	const float32 cy = std::cos(pitch);//attitude
	const float32 cz = std::cos(roll); //bank
	const float32 sx = std::sin(yaw);
	const float32 sy = std::sin(pitch);
	const float32 sz = std::sin(roll);

	Set(
				cx * cy, (-cx * sy * cz) + (sx * sz), (cx * sy * sz) + (sx * cz), 0.f,
				sy, cy * cz, -cy * sz, 0.f,
				-sx * cy, (sx * sy * cz) + (cx * sz), (-sx * sy * sz) + (cx * cz), 0.f,
				0.f, 0.f, 0.f, 1.f);
}

void AR_VEC_CALL Matrix4::SetScaling(float32 x, float32 y, float32 z)
{
	m_rows[0].Set(x, 0.f, 0.f, 0.f);
	m_rows[1].Set(0.f, y, 0.f, 0.f);
	m_rows[2].Set(0.f, 0.f, z, 0.f);
	m_rows[3].Set(0.f, 0.f, 0.f, 1.f);
}

void AR_VEC_CALL Matrix4::SetTranslation(float32 x, float32 y, float32 z)
{
	m_rows[0].Set(1.f, 0.f, 0.f, x);
	m_rows[1].Set(0.f, 1.f, 0.f, y);
	m_rows[2].Set(0.f, 0.f, 1.f, z);
	m_rows[3].Set(0.f, 0.f, 0.f, 1.f);
}

void AR_VEC_CALL Matrix4::SetTransformation(const Vector3 &euler,
	const Vector3 &scaling, const Vector3 &trans)
{
	Matrix4 m1, m2;
	const float32 cx = std::cos(euler[0]);
	const float32 cy = std::cos(euler[1]);
	const float32 cz = std::cos(euler[2]);
	const float32 sx = std::sin(euler[0]);
	const float32 sy = std::sin(euler[1]);
	const float32 sz = std::sin(euler[2]);

	m1.Set(
				cx * cy, (-cx * sy * cz) + (sx * sz), (cx * sy * sz) + (sx * cz), trans[0],
				sy, cy * cz, -cy * sz, trans[1],
				-sx * cy, (sx * sy * cz) + (cx * sz), (-sx * sy * sz) + (cx * cz), trans[2],
				0.f, 0.f, 0.f, 1.f);
	m2.SetScaling(scaling[0], scaling[1], scaling[2]);

	*this = (m1 * m2);
}

void AR_VEC_CALL Matrix4::SetTransformation(const Quaternion &rotation,
	const Vector3 &scaling, const Vector3 &trans)
{
	Matrix4 r(rotation), s, t;
	s.SetScaling(scaling[0], scaling[1], scaling[2]);
	t.SetTranslation(trans[0], trans[1], trans[2]);

	*this = t * r * s;
}

void AR_VEC_CALL Matrix4::SetRotationDeg(float32 roll, float32 pitch, float32 yaw)
{
	SetRotationRad(roll * c_rads_per_deg, pitch * c_rads_per_deg, yaw * c_rads_per_deg);
}

void AR_VEC_CALL Matrix4::SetRotation(const Quaternion &q)
{
	float32 s = q.Length();
	AR_ASSERT_MSG(s != 0.f, "Quaternion of length 0");
	s = 2.f / s;

#ifdef AR_SIMD
	__m128 row0, row1, row2;
	const __m128 qData = q.Get128();
	// Y Y W
	row0 = _mm_shuffle_ps(qData, qData, 0xA); //_MM_SHUFFLE(0, 0, 2, 2)
	// X X Z
	row1 = _mm_shuffle_ps(qData, qData, 0x35); //_MM_SHUFFLE(0, 3, 1, 1)
	// W W Y
	row2 = _mm_shuffle_ps(qData, qData, 0x20); //_MM_SHUFFLE(0, 2, 0, 0)

	// Y X Y Z
	__m128 temp = _mm_shuffle_ps(qData, qData, 0xE6); //_MM_SHUFFLE(3, 2, 1, 2)
	row0 = _mm_mul_ps(row0, temp);
	row1 = _mm_mul_ps(row1, temp);
	row2 = _mm_mul_ps(row2, temp);

	row0 = _mm_xor_ps(row0, _mm_set_ps(+0.f, +0.f, -0.f, +0.f));
	row1 = _mm_xor_ps(row1, _mm_set_ps(+0.f, -0.f, +0.f, +0.f));
	row2 = _mm_xor_ps(row2, _mm_set_ps(+0.f, +0.f, +0.f, -0.f));

	// Z Z X W
	temp = _mm_shuffle_ps(qData, qData, 0x1F); //_MM_SHUFFLE(0, 1, 3, 3)

	row0 = _mm_add_ps(row0,
		_mm_mul_ps(_mm_shuffle_ps(qData, qData, 0x33), temp)); //_MM_SHUFFLE(0, 3, 0, 3)
	row1 = _mm_add_ps(row1,
		_mm_mul_ps(_mm_shuffle_ps(qData, qData, 0x4C), temp)); //_MM_SHUFFLE(1, 0, 3, 0)
	row2 = _mm_add_ps(row2,
		_mm_mul_ps(_mm_shuffle_ps(qData, qData, 0x19), temp)); //_MM_SHUFFLE(0, 1, 2, 1)

	row0 = _mm_xor_ps(row0, _mm_set_ps(+0.f, +0.f, -0.f, +0.f));
	row1 = _mm_xor_ps(row1, _mm_set_ps(+0.f, -0.f, +0.f, +0.f));

	const __m128 scalar = _mm_set1_ps(s);
	row0 = _mm_mul_ps(scalar, row0);
	row1 = _mm_mul_ps(scalar, row1);
	row2 = _mm_mul_ps(scalar, row2);

	row0 = _mm_xor_ps(row0, _mm_set_ps(+0.f, +0.f, +0.f, -0.f));
	row1 = _mm_xor_ps(row1, _mm_set_ps(+0.f, +0.f, -0.f, +0.f));
	row2 = _mm_xor_ps(row2, _mm_set_ps(+0.f, -0.f, +0.f, +0.f));

	row0 = _mm_add_ps(_mm_set_ps(0.f, 0.f, 0.f, 1.f), row0);
	row1 = _mm_add_ps(_mm_set_ps(0.f, 0.f, 1.f, 0.f), row1);
	row2 = _mm_add_ps(_mm_set_ps(0.f, 1.f, 0.f, 0.f), row2);

	m_rows[0].Set(row0);
	m_rows[1].Set(row1);
	m_rows[2].Set(row2);
	m_rows[3].Set(0.f, 0.f, 0.f, 1.f);
	m_rows[0][3] = 0.f;
	m_rows[1][3] = 0.f;
	m_rows[2][3] = 0.f;
#else
	const float32 xy = s * q[1] * q[2];
	const float32 wz = s * q[0] * q[3];
	const float32 xz = s * q[1] * q[3];
	const float32 wy = s * q[0] * q[2];
	const float32 yz = s * q[2] * q[3];
	const float32 wx = s * q[0] * q[1];
	const float32 yy = s * q[2] * q[2];
	const float32 xx = s * q[1] * q[1];
	const float32 zz = s * q[3] * q[3];

	Set(
		1 - yy - zz, xy - wz, xz + wy, 0.f,
		xy + wz, 1 - xx - zz, yz - wx, 0.f,
		xz - wy, yz + wx, 1 - xx - yy, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
#endif // ifdef AR_SIMD
}

void AR_VEC_CALL Matrix4::GetQuaternion(Quaternion &q) const
{
	float32 trace = m_rows[0][0] + m_rows[1][1] + m_rows[2][2];

	if (trace > 0)
	{
		const float32 s = 0.5f / std::sqrt(trace + 1.f);
		q.Set(
			0.25f / s,
			(m_rows[2][1] - m_rows[1][2]) * s,
			(m_rows[0][2] - m_rows[2][0]) * s,
			(m_rows[1][0] - m_rows[0][1]) * s);
		return;
	}
	else if ((m_rows[0][0] > m_rows[1][1]) && (m_rows[0][0] > m_rows[2][2]))
	{
		const float32 s = std::sqrt(1.f + m_rows[0][0] - m_rows[1][1] - m_rows[2][2]) * 2;
		q.Set(
			(m_rows[2][1] - m_rows[1][2]) / s,
			0.25f * s,
			(m_rows[0][1] + m_rows[1][0]) / s,
			(m_rows[0][2] + m_rows[2][0]) / s);
		return;
	}
	else if (m_rows[1][1] > m_rows[2][2])
	{
		const float32 s = std::sqrt(1.f + m_rows[1][1] - m_rows[0][0] - m_rows[2][2]) * 2;
		q.Set(
			(m_rows[0][2] - m_rows[2][0]) / s,
			(m_rows[0][1] + m_rows[1][0]) / s,
			0.25f * s,
			(m_rows[1][2] + m_rows[2][1]) / s);
		return;
	}
	else
	{
		const float32 s = std::sqrt(1.f + m_rows[2][2] - m_rows[0][0] - m_rows[1][1]) * 2;
		q.Set(
			(m_rows[1][0] - m_rows[0][1]) / s,
			(m_rows[0][2] + m_rows[2][0]) / s,
			(m_rows[1][2] + m_rows[2][1]) / s,
			0.25f * s);
		return;
	}
}

void AR_VEC_CALL Matrix4::SetLookAtLH(const Vector3& eye, const Vector3& lookAt,
																			const Vector3& up)
{
	Vector3 zaxis = lookAt - eye;
	zaxis.Normalize();

	Vector3 xaxis = up.Cross(zaxis);
	xaxis.Normalize();

	const Vector3 yaxis = zaxis.Cross(xaxis);

	Set(
		xaxis[0], xaxis[1], xaxis[2], -xaxis.Dot(eye),
		yaxis[0], yaxis[1], yaxis[2], -yaxis.Dot(eye),
		zaxis[0], zaxis[1], zaxis[2], -zaxis.Dot(eye),
		0.f, 0.f, 0.f, 1.f);
}

void AR_VEC_CALL Matrix4::SetViewMatrix(const Vector3& right, const Vector3& up,
																				const Vector3& forward, const Vector3& eye)
{
	Set(
		right[0], right[1], right[2], right.Dot(-eye),
		up[0], up[1], up[2], up.Dot(-eye),
		forward[0], forward[1], forward[2], forward.Dot(-eye),
		0.f, 0.f, 0.f, 1.f
	);
}

void AR_VEC_CALL Matrix4::SetOrthographicLH(float32 width, float32 height,
																						float32 nearPlane, float32 farPlane)
{
	Set(
		2.f / width, 0.f, 0.f, 0.f,
		0.f, 2.f / height, 0.f, 0.f,
		0.f, 0.f, 1.f / (farPlane - nearPlane), nearPlane / (nearPlane - farPlane),
		0.f, 0.f, 0.f, 1.f
	);
}

void AR_VEC_CALL Matrix4::SetPerspectiveFovLH(float32 width, float32 height, float32 fov,
																							float32 nearPlane, float32 farPlane)
{
	const float32 aspect = width / height;
	const float32 yScale = 1.f / tanf(fov / 2.f);
	const float32 xScale = yScale / aspect;
	const float32 range = farPlane - nearPlane;

	Set(
		xScale, 0.f, 0.f, 0.f,
		0.f, yScale, 0.f, 0.f,
		0.f, 0.f, farPlane / range, -nearPlane * farPlane / range,
		0.f, 0.f, 1.f, 0.f
	);
}

float32 AR_VEC_CALL Matrix4::DeterminantFull(void) const
{
#ifdef AR_SIMD
	const __m128 row1 = _mm_shuffle_ps(m_rows[1].Get128(), m_rows[1].Get128(), 0x4E);
	const __m128 row3 = _mm_shuffle_ps(m_rows[3].Get128(), m_rows[3].Get128(), 0x4E);

	__m128 tmp = _mm_mul_ps(m_rows[2].Get128(), row3);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1); // _MM_SHUFFLE(2, 3, 0, 1)

	__m128 minor = _mm_mul_ps(tmp, row1);
	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E); //_MM_SHUFFLE(1, 0, 3, 2)
	minor = _mm_sub_ps(_mm_mul_ps(row1, tmp), minor);

	tmp = _mm_mul_ps(row1, m_rows[2].Get128());
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	minor = _mm_add_ps(_mm_mul_ps(row3, tmp), minor);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
	minor = _mm_sub_ps(minor, _mm_mul_ps(row3, tmp));

	tmp = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
	const __m128 row2 = _mm_shuffle_ps(m_rows[2].Get128(), m_rows[2].Get128(), 0x4E);
	minor = _mm_add_ps(_mm_mul_ps(row2, tmp), minor);
	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor = _mm_sub_ps(minor, _mm_mul_ps(row2, tmp));

	__m128 det = _mm_mul_ps(m_rows[0].Get128(), minor);
	det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
	det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);

	return _mm_cvtss_f32(det);
#else
	float32 c0 = m_rows[1][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		+ m_rows[3][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
	float32 c1 = m_rows[0][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
	float32 c2 = m_rows[0][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		- m_rows[1][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
	float32 c3 = m_rows[0][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
		- m_rows[1][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
		+ m_rows[2][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);

	return m_rows[0][0] * c0 - m_rows[1][0] * c1 + m_rows[2][0] * c2
		- m_rows[3][0] * c3;
#endif // ifdef AR_SIMD
}

float32 AR_VEC_CALL Matrix4::DeterminantFast(void) const
{
#ifdef AR_SIMD
	__m128 c1 = _mm_shuffle_ps(m_rows[1].Get128(), m_rows[1].Get128(), 0xC9);
	__m128 c2 = _mm_shuffle_ps(m_rows[2].Get128(), m_rows[2].Get128(), 0xC9); //_MM_SHUFFLE(3, 0, 2, 1)

	c1 = _mm_mul_ps(c1, m_rows[2].Get128());
	c2 = _mm_mul_ps(c2, m_rows[1].Get128());

	c1 = _mm_sub_ps(c2, c1);

	c1 = _mm_shuffle_ps(c1, c1, 0xC9);
	c1 = _mm_mul_ps(m_rows[0].Get128(), c1);

	const __m128 z = _mm_movehl_ps(c1, c1);
	const __m128 y = _mm_shuffle_ps(c1, c1, 0xFD);  //_MM_SHUFFLE(3, 3, 3, 1)
	c1 = _mm_add_ps(z, _mm_add_ps(y, c1));

	return _mm_cvtss_f32(c1);
#else
	return m_rows[0][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2])
		- m_rows[1][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2])
		+ m_rows[2][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);

#endif // ifdef AR_SIMD
}

Matrix4 AR_VEC_CALL Matrix4::Transposed(void) const
{
#ifdef AR_SIMD
	const __m128 temp1 = _mm_unpackhi_ps(m_rows[0].Get128(), m_rows[1].Get128());
	__m128 v0 = _mm_unpacklo_ps(m_rows[0].Get128(), m_rows[1].Get128());
	const __m128 temp2 = _mm_unpackhi_ps(m_rows[2].Get128(), m_rows[3].Get128());
	__m128 v2 = _mm_unpacklo_ps(m_rows[2].Get128(), m_rows[3].Get128());

	const __m128 v1 = _mm_shuffle_ps(v0, v2, 0xEE);
	const __m128 v3 = _mm_shuffle_ps(temp1, temp2, 0xEE); // _MM_SHUFFLE(3, 2, 3, 2)
	v0 = _mm_shuffle_ps(v0, v2, 0x44);
	v2 = _mm_shuffle_ps(temp1, temp2, 0x44); //_MM_SHUFFLE(1, 0, 1, 0)

	return Matrix4(v0, v1, v2, v3);
#else
	return Matrix4(m_rows[0][0], m_rows[1][0], m_rows[2][0], m_rows[3][0],
		m_rows[0][1], m_rows[1][1], m_rows[2][1], m_rows[3][1],
		m_rows[0][2], m_rows[1][2], m_rows[2][2], m_rows[3][2],
		m_rows[0][3], m_rows[1][3], m_rows[2][3], m_rows[3][3]);
#endif // ifdef AR_SIMD
}

Matrix4& AR_VEC_CALL Matrix4::Transpose(void)
{
#ifdef AR_SIMD
	const __m128 temp1 = _mm_unpackhi_ps(m_rows[0].Get128(), m_rows[1].Get128());
	const __m128 v0 = _mm_unpacklo_ps(m_rows[0].Get128(), m_rows[1].Get128());
	const __m128 temp2 = _mm_unpackhi_ps(m_rows[2].Get128(), m_rows[3].Get128());
	const __m128 v2 = _mm_unpacklo_ps(m_rows[2].Get128(), m_rows[3].Get128());

	m_rows[1].Set(_mm_shuffle_ps(v0, v2, 0xEE));
	m_rows[3].Set(_mm_shuffle_ps(temp1, temp2, 0xEE));
	m_rows[0].Set(_mm_shuffle_ps(v0, v2, 0x44));
	m_rows[2].Set(_mm_shuffle_ps(temp1, temp2, 0x44));
#else
	*this = Transposed();
#endif // ifdef AR_SIMD

	return *this;
}

Matrix4 AR_VEC_CALL Matrix4::GetCofactorMatrix(void) const
{
#ifdef AR_SIMD
	const __m128 row0 = m_rows[0].Get128();
	__m128 row1 = m_rows[1].Get128();
	__m128 row2 = m_rows[2].Get128();
	__m128 row3 = m_rows[3].Get128();

	row1 = _mm_shuffle_ps(row1, row1, 0x4E);
	row3 = _mm_shuffle_ps(row3, row3, 0x4E);

	__m128 tmp = _mm_mul_ps(row2, row3);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	__m128 minor0 = _mm_mul_ps(row1, tmp);
	__m128 minor1 = _mm_mul_ps(row0, tmp);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp), minor0);
	minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor1);
	minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

	tmp = _mm_mul_ps(row1, row2);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor0);
	__m128 minor3 = _mm_mul_ps(row0, tmp);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp));
	minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor3);
	minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

	tmp = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
	row2 = _mm_shuffle_ps(row2, row2, 0x4E);

	minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor0);
	__m128 minor2 = _mm_mul_ps(row0, tmp);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp));
	minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor2);
	minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

	tmp = _mm_mul_ps(row0, row1);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor2);
	minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp), minor3);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp), minor2);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp));

	tmp = _mm_mul_ps(row0, row3);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp));
	minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor2);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor1);
	minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp));

	tmp = _mm_mul_ps(row0, row2);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor1);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp));

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp));
	minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor3);

	return Matrix4(minor0, minor1, minor2, minor3);
#else
	const float32 c00
		= m_rows[1][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		+ m_rows[3][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
	const float32 c01
		= m_rows[1][0] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][0] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		+ m_rows[3][0] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
	const float32 c02
		= m_rows[1][0] * (m_rows[2][1] * m_rows[3][3] - m_rows[3][1] * m_rows[2][3])
		- m_rows[2][0] * (m_rows[1][1] * m_rows[3][3] - m_rows[3][1] * m_rows[1][3])
		+ m_rows[3][0] * (m_rows[1][1] * m_rows[2][3] - m_rows[2][1] * m_rows[1][3]);
	const float32 c03
		= m_rows[1][0] * (m_rows[2][1] * m_rows[3][2] - m_rows[3][1] * m_rows[2][2])
		- m_rows[2][0] * (m_rows[1][1] * m_rows[3][2] - m_rows[3][1] * m_rows[1][2])
		+ m_rows[3][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2]);
	const float32 c10
		= m_rows[0][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
	const float32 c11
		= m_rows[0][0] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][0] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][0] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
	const float32 c12
		= m_rows[0][0] * (m_rows[2][1] * m_rows[3][3] - m_rows[3][1] * m_rows[2][3])
		- m_rows[2][0] * (m_rows[0][1] * m_rows[3][3] - m_rows[3][1] * m_rows[0][3])
		+ m_rows[3][0] * (m_rows[0][1] * m_rows[2][3] - m_rows[2][1] * m_rows[0][3]);
	const float32 c13
		= m_rows[0][0] * (m_rows[2][1] * m_rows[3][2] - m_rows[3][1] * m_rows[2][2])
		- m_rows[2][0] * (m_rows[0][1] * m_rows[3][2] - m_rows[3][1] * m_rows[0][2])
		+ m_rows[3][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2]);
	const float32 c20
		= m_rows[0][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		- m_rows[1][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
	const float32 c21
		= m_rows[0][0] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		- m_rows[1][0] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][0] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
	const float32 c22
		= m_rows[0][0] * (m_rows[1][1] * m_rows[3][3] - m_rows[3][1] * m_rows[1][3])
		- m_rows[1][0] * (m_rows[0][1] * m_rows[3][3] - m_rows[3][1] * m_rows[0][3])
		+ m_rows[3][0] * (m_rows[0][1] * m_rows[1][3] - m_rows[1][1] * m_rows[0][3]);
	const float32 c23
		= m_rows[0][0] * (m_rows[1][1] * m_rows[3][2] - m_rows[3][1] * m_rows[1][2])
		- m_rows[1][0] * (m_rows[0][1] * m_rows[3][2] - m_rows[3][1] * m_rows[0][2])
		+ m_rows[3][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);
	const float32 c30
		= m_rows[0][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
		- m_rows[1][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
		+ m_rows[2][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
	const float32 c31
		= m_rows[0][0] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
		- m_rows[1][0] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
		+ m_rows[2][0] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
	const float32 c32
		= m_rows[0][0] * (m_rows[1][1] * m_rows[2][3] - m_rows[2][1] * m_rows[1][3])
		- m_rows[1][0] * (m_rows[0][1] * m_rows[2][3] - m_rows[2][1] * m_rows[0][3])
		+ m_rows[2][0] * (m_rows[0][1] * m_rows[1][3] - m_rows[1][1] * m_rows[0][3]);
	const float32 c33
		= m_rows[0][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2])
		- m_rows[1][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2])
		+ m_rows[2][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);

	return Matrix4(c00, -c01, c02, -c03,
		-c10, c11, -c12, c13,
		c20, -c21, c22, -c23,
		-c30, c31, -c32, c33);
#endif // ifdef AR_SIMD
}

Matrix4 AR_VEC_CALL Matrix4::GetAdjointMatrix(void) const
{
#ifdef AR_SIMD
	const __m128 row0 = m_rows[0].Get128();
	__m128 row1 = m_rows[1].Get128();
	__m128 row2 = m_rows[2].Get128();
	__m128 row3 = m_rows[3].Get128();

	row1 = _mm_shuffle_ps(row1, row1, 0x4E);
	row3 = _mm_shuffle_ps(row3, row3, 0x4E);

	__m128 tmp = _mm_mul_ps(row2, row3);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	__m128 minor0 = _mm_mul_ps(row1, tmp);
	__m128 minor1 = _mm_mul_ps(row0, tmp);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp), minor0);
	minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor1);
	minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

	tmp = _mm_mul_ps(row1, row2);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor0);
	__m128 minor3 = _mm_mul_ps(row0, tmp);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp));
	minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor3);
	minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

	tmp = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
	row2 = _mm_shuffle_ps(row2, row2, 0x4E);

	minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor0);
	__m128 minor2 = _mm_mul_ps(row0, tmp);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp));
	minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor2);
	minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

	tmp = _mm_mul_ps(row0, row1);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor2);
	minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp), minor3);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp), minor2);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp));

	tmp = _mm_mul_ps(row0, row3);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp));
	minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor2);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor1);
	minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp));

	tmp = _mm_mul_ps(row0, row2);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor1);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp));

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp));
	minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor3);

	__m128 temp1 = _mm_unpackhi_ps(minor0, minor1);
	__m128 v0 = _mm_unpacklo_ps(minor0, minor1);
	__m128 temp2 = _mm_unpackhi_ps(minor2, minor3);
	__m128 v2 = _mm_unpacklo_ps(minor2, minor3);

	return Matrix4(_mm_shuffle_ps(v0, v2, 0x44), _mm_shuffle_ps(v0, v2, 0xEE),
		_mm_shuffle_ps(temp1, temp2, 0x44), _mm_shuffle_ps(temp1, temp2, 0xEE));
#else
	const float32 c00
		= m_rows[1][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		+ m_rows[3][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
	const float32 c01
		= m_rows[1][0] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][0] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		+ m_rows[3][0] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
	const float32 c02
		= m_rows[1][0] * (m_rows[2][1] * m_rows[3][3] - m_rows[3][1] * m_rows[2][3])
		- m_rows[2][0] * (m_rows[1][1] * m_rows[3][3] - m_rows[3][1] * m_rows[1][3])
		+ m_rows[3][0] * (m_rows[1][1] * m_rows[2][3] - m_rows[2][1] * m_rows[1][3]);
	const float32 c03
		= m_rows[1][0] * (m_rows[2][1] * m_rows[3][2] - m_rows[3][1] * m_rows[2][2])
		- m_rows[2][0] * (m_rows[1][1] * m_rows[3][2] - m_rows[3][1] * m_rows[1][2])
		+ m_rows[3][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2]);
	const float32 c10
		= m_rows[0][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
	const float32 c11
		= m_rows[0][0] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][0] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][0] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
	const float32 c12
		= m_rows[0][0] * (m_rows[2][1] * m_rows[3][3] - m_rows[3][1] * m_rows[2][3])
		- m_rows[2][0] * (m_rows[0][1] * m_rows[3][3] - m_rows[3][1] * m_rows[0][3])
		+ m_rows[3][0] * (m_rows[0][1] * m_rows[2][3] - m_rows[2][1] * m_rows[0][3]);
	const float32 c13
		= m_rows[0][0] * (m_rows[2][1] * m_rows[3][2] - m_rows[3][1] * m_rows[2][2])
		- m_rows[2][0] * (m_rows[0][1] * m_rows[3][2] - m_rows[3][1] * m_rows[0][2])
		+ m_rows[3][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2]);
	const float32 c20
		= m_rows[0][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		- m_rows[1][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
	const float32 c21
		= m_rows[0][0] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		- m_rows[1][0] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][0] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
	const float32 c22
		= m_rows[0][0] * (m_rows[1][1] * m_rows[3][3] - m_rows[3][1] * m_rows[1][3])
		- m_rows[1][0] * (m_rows[0][1] * m_rows[3][3] - m_rows[3][1] * m_rows[0][3])
		+ m_rows[3][0] * (m_rows[0][1] * m_rows[1][3] - m_rows[1][1] * m_rows[0][3]);
	const float32 c23
		= m_rows[0][0] * (m_rows[1][1] * m_rows[3][2] - m_rows[3][1] * m_rows[1][2])
		- m_rows[1][0] * (m_rows[0][1] * m_rows[3][2] - m_rows[3][1] * m_rows[0][2])
		+ m_rows[3][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);
	const float32 c30
		= m_rows[0][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
		- m_rows[1][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
		+ m_rows[2][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
	const float32 c31
		= m_rows[0][0] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
		- m_rows[1][0] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
		+ m_rows[2][0] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
	const float32 c32
		= m_rows[0][0] * (m_rows[1][1] * m_rows[2][3] - m_rows[2][1] * m_rows[1][3])
		- m_rows[1][0] * (m_rows[0][1] * m_rows[2][3] - m_rows[2][1] * m_rows[0][3])
		+ m_rows[2][0] * (m_rows[0][1] * m_rows[1][3] - m_rows[1][1] * m_rows[0][3]);
	const float32 c33
		= m_rows[0][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2])
		- m_rows[1][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2])
		+ m_rows[2][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);

	return Matrix4(c00, -c10, c20, -c30,
		-c01, c11, -c21, c31,
		c02, -c12, c22, -c32,
		-c03, c13, -c23, c33);
#endif // ifdef AR_SIMD
}

Matrix4 AR_VEC_CALL Matrix4::Inverted(void) const
{
	AR_ASSERT_MSG(DeterminantFull() != 0.f, "Determinant is zero");

#ifdef AR_SIMD
	const __m128 temp1 = _mm_unpackhi_ps(m_rows[0].Get128(), m_rows[1].Get128());
	const __m128 v0 = _mm_unpacklo_ps(m_rows[0].Get128(), m_rows[1].Get128());
	const __m128 temp2 = _mm_unpackhi_ps(m_rows[2].Get128(), m_rows[3].Get128());
	const __m128 v2 = _mm_unpacklo_ps(m_rows[2].Get128(), m_rows[3].Get128());

	__m128 row1 = _mm_shuffle_ps(v0, v2, 0xEE);
	__m128 row3 = _mm_shuffle_ps(temp1, temp2, 0xEE);
	const __m128 row0 = _mm_shuffle_ps(v0, v2, 0x44);
	__m128 row2 = _mm_shuffle_ps(temp1, temp2, 0x44);

	row1 = _mm_shuffle_ps(row1, row1, 0x4E);
	row3 = _mm_shuffle_ps(row3, row3, 0x4E);

	__m128 tmp = _mm_mul_ps(row2, row3);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	__m128 minor0 = _mm_mul_ps(row1, tmp);
	__m128 minor1 = _mm_mul_ps(row0, tmp);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp), minor0);
	minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor1);
	minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

	tmp = _mm_mul_ps(row1, row2);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor0);
	__m128 minor3 = _mm_mul_ps(row0, tmp);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp));
	minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor3);
	minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

	tmp = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
	row2 = _mm_shuffle_ps(row2, row2, 0x4E);

	minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor0);
	__m128 minor2 = _mm_mul_ps(row0, tmp);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp));
	minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor2);
	minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

	tmp = _mm_mul_ps(row0, row1);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor2);
	minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp), minor3);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp), minor2);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp));

	tmp = _mm_mul_ps(row0, row3);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp));
	minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor2);

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor1);
	minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp));

	tmp = _mm_mul_ps(row0, row2);
	tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

	minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor1);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp));

	tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp));
	minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor3);

	__m128 det = _mm_mul_ps(row0, minor0);
	det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
	det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
	tmp = _mm_rcp_ss(det);
	det = _mm_sub_ss(_mm_add_ss(tmp, tmp), _mm_mul_ss(det, _mm_mul_ss(tmp, tmp)));
	det = _mm_shuffle_ps(det, det, 0x00);

	minor0 = _mm_mul_ps(det, minor0);
	minor1 = _mm_mul_ps(det, minor1);
	minor2 = _mm_mul_ps(det, minor2);
	minor3 = _mm_mul_ps(det, minor3);

	return Matrix4(minor0, minor1, minor2, minor3);
#else
	const float32 c00
		= m_rows[1][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		+ m_rows[3][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
	float32 c01
		= m_rows[1][0] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][0] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		+ m_rows[3][0] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
	float32 c02
		= m_rows[1][0] * (m_rows[2][1] * m_rows[3][3] - m_rows[3][1] * m_rows[2][3])
		- m_rows[2][0] * (m_rows[1][1] * m_rows[3][3] - m_rows[3][1] * m_rows[1][3])
		+ m_rows[3][0] * (m_rows[1][1] * m_rows[2][3] - m_rows[2][1] * m_rows[1][3]);
	float32 c03
		= m_rows[1][0] * (m_rows[2][1] * m_rows[3][2] - m_rows[3][1] * m_rows[2][2])
		- m_rows[2][0] * (m_rows[1][1] * m_rows[3][2] - m_rows[3][1] * m_rows[1][2])
		+ m_rows[3][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2]);
	float32 c10
		= m_rows[0][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
	float32 c11
		= m_rows[0][0] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
		- m_rows[2][0] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][0] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
	float32 c12
		= m_rows[0][0] * (m_rows[2][1] * m_rows[3][3] - m_rows[3][1] * m_rows[2][3])
		- m_rows[2][0] * (m_rows[0][1] * m_rows[3][3] - m_rows[3][1] * m_rows[0][3])
		+ m_rows[3][0] * (m_rows[0][1] * m_rows[2][3] - m_rows[2][1] * m_rows[0][3]);
	float32 c13
		= m_rows[0][0] * (m_rows[2][1] * m_rows[3][2] - m_rows[3][1] * m_rows[2][2])
		- m_rows[2][0] * (m_rows[0][1] * m_rows[3][2] - m_rows[3][1] * m_rows[0][2])
		+ m_rows[3][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2]);
	float32 c20
		= m_rows[0][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		- m_rows[1][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
	float32 c21
		= m_rows[0][0] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		- m_rows[1][0] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][0] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[1][3]);
	float32 c22
		= m_rows[0][0] * (m_rows[1][1] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
		- m_rows[1][0] * (m_rows[0][1] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
		+ m_rows[3][0] * (m_rows[0][1] * m_rows[1][3] - m_rows[1][1] * m_rows[0][3]);
	float32 c23
		= m_rows[0][0] * (m_rows[1][1] * m_rows[3][2] - m_rows[3][2] * m_rows[1][2])
		- m_rows[1][0] * (m_rows[0][1] * m_rows[3][2] - m_rows[3][2] * m_rows[0][2])
		+ m_rows[3][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);
	float32 c30
		= m_rows[0][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
		- m_rows[1][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
		+ m_rows[2][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
	float32 c31
		= m_rows[0][0] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
		- m_rows[1][0] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
		+ m_rows[2][0] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
	float32 c32
		= m_rows[0][0] * (m_rows[1][1] * m_rows[2][3] - m_rows[2][1] * m_rows[1][3])
		- m_rows[1][0] * (m_rows[0][1] * m_rows[2][3] - m_rows[2][1] * m_rows[0][3])
		+ m_rows[2][0] * (m_rows[0][1] * m_rows[1][3] - m_rows[1][1] * m_rows[1][3]);
	float32 c33
		= m_rows[0][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2])
		- m_rows[1][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2])
		+ m_rows[2][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);

	const float32 det = 1.f / (m_rows[0][0] * c00 - m_rows[1][0] * c10
		+ m_rows[2][0] * c20 - m_rows[3][0] * c30);

	return Matrix4(c00 * det, -c10 * det, c20 * det, -c30 * det,
		-c01 * det, c11 * det, -c21 * det, c31 * det,
		c02 * det, -c12 * det, c22 * det, -c32 * det,
		-c03 * det, c13* det, -c23 * det, c33 * det);
#endif // ifdef AR_SIMD
}

Matrix4& AR_VEC_CALL Matrix4::Inverse(void)
{
	*this = Inverted();

	return *this;
}

Vector3 AR_VEC_CALL Matrix4::TransformNormal(const Vector3 &n) const
{
#ifdef AR_SIMD
	const __m128 v = _mm_set_ps(0.0, n[2], n[1], n[0]);
	__m128 mx = _mm_mul_ps(m_rows[0].Get128(), v);
	__m128 w = _mm_shuffle_ps(mx, mx, 0xFF); //_MM_SHUFFLE(3, 3, 3, 3)
	__m128 z = _mm_shuffle_ps(mx, mx, 0xAA); //_MM_SHUFFLE(2, 2, 2, 2)
	__m128 y = _mm_shuffle_ps(mx, mx, 0x55); //_MM_SHUFFLE(1, 1, 1, 1)

	mx = _mm_add_ps(mx, w);
	mx = _mm_add_ps(mx, z);
	mx = _mm_add_ps(mx, y);

	__m128 my = _mm_mul_ps(m_rows[1].Get128(), v);
	w = _mm_shuffle_ps(my, my, 0xFF); //_MM_SHUFFLE(3, 3, 3, 3)
	z = _mm_shuffle_ps(my, my, 0xAA); //_MM_SHUFFLE(2, 2, 2, 2)
	y = _mm_shuffle_ps(my, my, 0x55); //_MM_SHUFFLE(1, 1, 1, 1)

	my = _mm_add_ps(my, w);
	my = _mm_add_ps(my, z);
	my = _mm_add_ps(my, y);

	__m128 mz = _mm_mul_ps(m_rows[2].Get128(), v);
	w = _mm_shuffle_ps(mz, mz, 0xFF); //_MM_SHUFFLE(3, 3, 3, 3)
	z = _mm_shuffle_ps(mz, mz, 0xAA); //_MM_SHUFFLE(2, 2, 2, 2)
	y = _mm_shuffle_ps(mz, mz, 0x55); //_MM_SHUFFLE(1, 1, 1, 1)

	mz = _mm_add_ps(mz, w);
	mz = _mm_add_ps(mz, z);
	mz = _mm_add_ps(mz, y);

	return Vector3(_mm_cvtss_f32(mx), _mm_cvtss_f32(my), _mm_cvtss_f32(mz));
#else
	return Vector3(m_rows[0][0] * n[0] + m_rows[0][1] * n[1] + m_rows[0][2] * n[2],
		m_rows[1][0] * n[0] + m_rows[1][1] * n[1] + m_rows[1][2] * n[2],
		m_rows[2][0] * n[0] + m_rows[2][1] * n[1] + m_rows[2][2] * n[2]);
#endif // ifdef AR_SIMD
}

Vector3 AR_VEC_CALL Matrix4::GetColumn(int32 i) const
{
	AR_ASSERT_MSG(0 <= i && i < 4, "Index is out of range");

	return Vector3(m_rows[0][i], m_rows[1][i], m_rows[2][i]);
}

void AR_VEC_CALL Matrix4::SetColumn(int32 i, const Vector4 &v)
{
	AR_ASSERT_MSG(0 <= i && i < 4, "Index is out of range");

	m_rows[0][i] = v[0];
	m_rows[1][i] = v[1];
	m_rows[2][i] = v[2];
	m_rows[3][i] = v[3];
}

Vector4& AR_VEC_CALL Matrix4::operator[](int32 i)
{
	AR_ASSERT_MSG(0 <= i && i < 4, "Index is out of range");

	return m_rows[i];
}

const Vector4& AR_VEC_CALL Matrix4::operator[](int32 i) const
{
	AR_ASSERT_MSG(0 <= i && i < 4, "Index is out of range");

	return m_rows[i];
}

Matrix4 AR_VEC_CALL Matrix4::operator+(const Matrix4 &other) const
{
	return Matrix4(m_rows[0] + other.m_rows[0], m_rows[1] + other.m_rows[1],
		m_rows[2] + other.m_rows[2], m_rows[3] + other.m_rows[3]);
}

Matrix4& AR_VEC_CALL Matrix4::operator+=(const Matrix4 &other)
{
	m_rows[0] += other.m_rows[0];
	m_rows[1] += other.m_rows[1];
	m_rows[2] += other.m_rows[2];
	m_rows[3] += other.m_rows[3];

	return *this;
}

Matrix4 AR_VEC_CALL Matrix4::operator-(const Matrix4 &other) const
{
	return Matrix4(m_rows[0] - other.m_rows[0], m_rows[1] - other.m_rows[1],
		m_rows[2] - other.m_rows[2], m_rows[3] - other.m_rows[3]);
}

Matrix4& AR_VEC_CALL Matrix4::operator-=(const Matrix4 &other)
{
	m_rows[0] -= other.m_rows[0];
	m_rows[1] -= other.m_rows[1];
	m_rows[2] -= other.m_rows[2];
	m_rows[3] -= other.m_rows[3];

	return *this;
}

Matrix4 AR_VEC_CALL Matrix4::operator*(const Matrix4 &other) const
{
#ifdef AR_SIMD
	const __m128 tv0 = m_rows[0].Get128();
	const __m128 tv1 = m_rows[1].Get128();
	const __m128 tv2 = m_rows[2].Get128();
	const __m128 tv3 = m_rows[3].Get128();
	const __m128 mv0 = other.m_rows[0].Get128();
	const __m128 mv1 = other.m_rows[1].Get128();
	const __m128 mv2 = other.m_rows[2].Get128();
	const __m128 mv3 = other.m_rows[3].Get128();

	__m128 v00 = _mm_shuffle_ps(tv0, tv0, 0x0);
	__m128 v01 = _mm_shuffle_ps(tv0, tv0, 0x55);
	__m128 v02 = _mm_shuffle_ps(tv0, tv0, 0xAA);
	__m128 v03 = _mm_shuffle_ps(tv0, tv0, 0xFF);

	v00 = _mm_mul_ps(v00, mv0);
	v01 = _mm_mul_ps(v01, mv1);
	v02 = _mm_mul_ps(v02, mv2);
	v03 = _mm_mul_ps(v03, mv3);

	__m128 v10 = _mm_shuffle_ps(tv1, tv1, 0x0);
	__m128 v11 = _mm_shuffle_ps(tv1, tv1, 0x55);
	__m128 v12 = _mm_shuffle_ps(tv1, tv1, 0xAA);
	__m128 v13 = _mm_shuffle_ps(tv1, tv1, 0xFF);

	v10 = _mm_mul_ps(v10, mv0);
	v11 = _mm_mul_ps(v11, mv1);
	v12 = _mm_mul_ps(v12, mv2);
	v13 = _mm_mul_ps(v13, mv3);

	__m128 v20 = _mm_shuffle_ps(tv2, tv2, 0x0);
	__m128 v21 = _mm_shuffle_ps(tv2, tv2, 0x55);
	__m128 v22 = _mm_shuffle_ps(tv2, tv2, 0xAA);
	__m128 v23 = _mm_shuffle_ps(tv2, tv2, 0xFF);

	v20 = _mm_mul_ps(v20, mv0);
	v21 = _mm_mul_ps(v21, mv1);
	v22 = _mm_mul_ps(v22, mv2);
	v23 = _mm_mul_ps(v23, mv3);

	__m128 v30 = _mm_shuffle_ps(tv3, tv3, 0x0);
	__m128 v31 = _mm_shuffle_ps(tv3, tv3, 0x55);
	__m128 v32 = _mm_shuffle_ps(tv3, tv3, 0xAA);
	__m128 v33 = _mm_shuffle_ps(tv3, tv3, 0xFF);

	v30 = _mm_mul_ps(v30, mv0);
	v31 = _mm_mul_ps(v31, mv1);
	v32 = _mm_mul_ps(v32, mv2);
	v33 = _mm_mul_ps(v33, mv3);

	v00 = _mm_add_ps(v00, v01);
	v02 = _mm_add_ps(v02, v03);
	v00 = _mm_add_ps(v00, v02);

	v10 = _mm_add_ps(v10, v11);
	v12 = _mm_add_ps(v12, v13);
	v10 = _mm_add_ps(v10, v12);

	v20 = _mm_add_ps(v20, v21);
	v22 = _mm_add_ps(v22, v23);
	v20 = _mm_add_ps(v20, v22);

	v30 = _mm_add_ps(v30, v31);
	v32 = _mm_add_ps(v32, v33);
	v30 = _mm_add_ps(v30, v32);

	return Matrix4(v00, v10, v20, v30);
#else
	return Matrix4(m_rows[0].Dot(other._GetColumn4(0)), m_rows[0].Dot(other._GetColumn4(1)),
		m_rows[0].Dot(other._GetColumn4(2)), m_rows[0].Dot(other._GetColumn4(3)),
		m_rows[1].Dot(other._GetColumn4(0)), m_rows[1].Dot(other._GetColumn4(1)),
		m_rows[1].Dot(other._GetColumn4(2)), m_rows[1].Dot(other._GetColumn4(3)),
		m_rows[2].Dot(other._GetColumn4(0)), m_rows[2].Dot(other._GetColumn4(1)),
		m_rows[2].Dot(other._GetColumn4(2)), m_rows[2].Dot(other._GetColumn4(3)),
		m_rows[3].Dot(other._GetColumn4(0)), m_rows[3].Dot(other._GetColumn4(1)),
		m_rows[3].Dot(other._GetColumn4(2)), m_rows[3].Dot(other._GetColumn4(3)));
#endif // ifdef AR_SIMD
}

Matrix4& AR_VEC_CALL Matrix4::operator*=(const Matrix4 &other)
{
#ifdef AR_SIMD
	const __m128 tv0 = m_rows[0].Get128();
	const __m128 tv1 = m_rows[1].Get128();
	const __m128 tv2 = m_rows[2].Get128();
	const __m128 tv3 = m_rows[3].Get128();
	const __m128 mv0 = other.m_rows[0].Get128();
	const __m128 mv1 = other.m_rows[1].Get128();
	const __m128 mv2 = other.m_rows[2].Get128();
	const __m128 mv3 = other.m_rows[3].Get128();

	__m128 v00 = _mm_shuffle_ps(tv0, tv0, 0x0);
	__m128 v01 = _mm_shuffle_ps(tv0, tv0, 0x55);
	__m128 v02 = _mm_shuffle_ps(tv0, tv0, 0xAA);
	__m128 v03 = _mm_shuffle_ps(tv0, tv0, 0xFF);

	v00 = _mm_mul_ps(v00, mv0);
	v01 = _mm_mul_ps(v01, mv1);
	v02 = _mm_mul_ps(v02, mv2);
	v03 = _mm_mul_ps(v03, mv3);

	__m128 v10 = _mm_shuffle_ps(tv1, tv1, 0x0);
	__m128 v11 = _mm_shuffle_ps(tv1, tv1, 0x55);
	__m128 v12 = _mm_shuffle_ps(tv1, tv1, 0xAA);
	__m128 v13 = _mm_shuffle_ps(tv1, tv1, 0xFF);

	v10 = _mm_mul_ps(v10, mv0);
	v11 = _mm_mul_ps(v11, mv1);
	v12 = _mm_mul_ps(v12, mv2);
	v13 = _mm_mul_ps(v13, mv3);

	__m128 v20 = _mm_shuffle_ps(tv2, tv2, 0x0);
	__m128 v21 = _mm_shuffle_ps(tv2, tv2, 0x55);
	__m128 v22 = _mm_shuffle_ps(tv2, tv2, 0xAA);
	__m128 v23 = _mm_shuffle_ps(tv2, tv2, 0xFF);

	v20 = _mm_mul_ps(v20, mv0);
	v21 = _mm_mul_ps(v21, mv1);
	v22 = _mm_mul_ps(v22, mv2);
	v23 = _mm_mul_ps(v23, mv3);

	__m128 v30 = _mm_shuffle_ps(tv3, tv3, 0x0);
	__m128 v31 = _mm_shuffle_ps(tv3, tv3, 0x55);
	__m128 v32 = _mm_shuffle_ps(tv3, tv3, 0xAA);
	__m128 v33 = _mm_shuffle_ps(tv3, tv3, 0xFF);

	v30 = _mm_mul_ps(v30, mv0);
	v31 = _mm_mul_ps(v31, mv1);
	v32 = _mm_mul_ps(v32, mv2);
	v33 = _mm_mul_ps(v33, mv3);

	v00 = _mm_add_ps(v00, v01);
	v02 = _mm_add_ps(v02, v03);
	m_rows[0].Set(_mm_add_ps(v00, v02));

	v10 = _mm_add_ps(v10, v11);
	v12 = _mm_add_ps(v12, v13);
	m_rows[1].Set(_mm_add_ps(v10, v12));

	v20 = _mm_add_ps(v20, v21);
	v22 = _mm_add_ps(v22, v23);
	m_rows[2].Set(_mm_add_ps(v20, v22));

	v30 = _mm_add_ps(v30, v31);
	v32 = _mm_add_ps(v32, v33);
	m_rows[3].Set(_mm_add_ps(v30, v32));
#else
	m_rows[0].Set(m_rows[0].Dot(other._GetColumn4(0)), m_rows[0].Dot(other._GetColumn4(1)),
		m_rows[0].Dot(other._GetColumn4(2)), m_rows[0].Dot(other._GetColumn4(3)));
	m_rows[1].Set(m_rows[1].Dot(other._GetColumn4(0)), m_rows[1].Dot(other._GetColumn4(1)),
		m_rows[1].Dot(other._GetColumn4(2)), m_rows[1].Dot(other._GetColumn4(3)));
	m_rows[2].Set(m_rows[2].Dot(other._GetColumn4(0)), m_rows[2].Dot(other._GetColumn4(1)),
		m_rows[2].Dot(other._GetColumn4(2)), m_rows[2].Dot(other._GetColumn4(3)));
	m_rows[3].Set(m_rows[3].Dot(other._GetColumn4(0)), m_rows[3].Dot(other._GetColumn4(1)),
		m_rows[3].Dot(other._GetColumn4(2)), m_rows[3].Dot(other._GetColumn4(3)));
#endif // ifdef AR_SIMD

	return *this;
}

Matrix4 AR_VEC_CALL Matrix4::operator*(float32 s) const
{
	return Matrix4(m_rows[0] * s, m_rows[1] * s, m_rows[2] * s, m_rows[3] * s);
}

Vector3 AR_VEC_CALL Matrix4::operator*(const Vector3 &v) const
{
	return Vector3(m_rows[0].Dot(v), m_rows[1].Dot(v), m_rows[2].Dot(v));
}

Vector4 AR_VEC_CALL Matrix4::operator*(const Vector4 &v) const
{
	return Vector4(m_rows[0].Dot(v), m_rows[1].Dot(v), m_rows[2].Dot(v), m_rows[3].Dot(v));
}

bool AR_VEC_CALL Matrix4::operator==(const Matrix4 &other) const
{
	return (m_rows[0] == other.m_rows[0]) && (m_rows[1] == other.m_rows[1])
		&& (m_rows[2] == other.m_rows[2]) && (m_rows[3] == other.m_rows[3]);
}

bool AR_VEC_CALL Matrix4::operator!=(const Matrix4 &other) const
{
	return !(*this == other);
}

Vector4 AR_VEC_CALL Matrix4::_GetColumn4(int32 i) const
{
	AR_ASSERT_MSG(0 <= i && i < 4, "Index is out of range");
	return Vector4(m_rows[0][i], m_rows[1][i], m_rows[2][i], m_rows[3][i]);
}
} // namespace Argon
