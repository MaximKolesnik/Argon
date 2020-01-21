#include <cmath>
#include <utility>

#include <fundamental/debug.hpp>

#include "constants.hpp"
#include "matrix3.hpp"
#include "quaternion.hpp"

namespace Argon::Math
{
const Matrix3 AR_ATTR_ALIGN(16) Matrix3::c_identity(Vector3::c_xAxis, Vector3::c_yAxis,
																										Vector3::c_zAxis);

Matrix3::Matrix3()
{
}

AR_VEC_CALL Matrix3::Matrix3(const Matrix3 &other)
{
	m_rows[0] = other.m_rows[0];
	m_rows[1] = other.m_rows[1];
	m_rows[2] = other.m_rows[2];
}

AR_VEC_CALL Matrix3::Matrix3(const Quaternion &q)
{
	float32 s = q.getLengthSq();
	AR_ASSERT_MSG(s != 0, "Quaternion of length 0");
	s = 2.f / s;

#ifdef AR_SIMD
	__m128 row0, row1, row2;
	const __m128 qData = q.get128();
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

	m_rows[0].set(row0);
	m_rows[1].set(row1);
	m_rows[2].set(row2);
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

	set(
		1 - yy - zz, xy - wz, xz + wy,
		xy + wz, 1 - xx - zz, yz - wx,
		xz - wy, yz + wx, 1 - xx - yy
		);
#endif // ifdef AR_SIMD
}

AR_VEC_CALL Matrix3::Matrix3(
		float32 e00, float32 e01, float32 e02,
		float32 e10, float32 e11, float32 e12,
		float32 e20, float32 e21, float32 e22)
{
	m_rows[0].set(e00, e01, e02);
	m_rows[1].set(e10, e11, e12);
	m_rows[2].set(e20, e21, e22);
}

AR_VEC_CALL Matrix3::Matrix3(const Vector3 &v0, const Vector3 &v1,
														 const Vector3 &v2)
{
	m_rows[0] = v0;
	m_rows[1] = v1;
	m_rows[2] = v2;
}

#ifdef AR_SIMD
AR_VEC_CALL Matrix3::Matrix3(__m128 v0, __m128 v1, __m128 v2)
{
	m_rows[0] = Vector3(v0);
	m_rows[1] = Vector3(v1);
	m_rows[2] = Vector3(v2);
}
#endif // ifdef AR_SIMD

Matrix3& AR_VEC_CALL Matrix3::operator=(const Matrix3 &other)
{
	m_rows[0] = other.m_rows[0];
	m_rows[1] = other.m_rows[1];
	m_rows[2] = other.m_rows[2];

	return *this;
}

void AR_VEC_CALL Matrix3::set(float32 e00, float32 e01, float32 e02,
															float32 e10, float32 e11, float32 e12,
															float32 e20, float32 e21, float32 e22)
{
	m_rows[0].set(e00, e01, e02);
	m_rows[1].set(e10, e11, e12);
	m_rows[2].set(e20, e21, e22);
}

void Matrix3::setIdentity()
{
	m_rows[0] = Vector3::c_xAxis;
	m_rows[1] = Vector3::c_yAxis;
	m_rows[2] = Vector3::c_zAxis;
}

void Matrix3::zeroOut(void)
{
	m_rows[0].zeroOut();
	m_rows[1].zeroOut();
	m_rows[2].zeroOut();
}

void AR_VEC_CALL Matrix3::setRotationRad(float32 x, float32 y, float32 z)
{
	const float32 cx = std::cos(x);
	const float32 cy = std::cos(y);
	const float32 cz = std::cos(z);
	const float32 sx = std::sin(x);
	const float32 sy = std::sin(y);
	const float32 sz = std::sin(z);
	const float32 sxz = sx * sz;
	const float32 csxz = cz * sx;

	set( (cz * cy) + (sxz * sy), (-sz * cy) + (csxz * sy), cx * sy,
											sz * cx,									cz * cx,		 -sx,
			(cz * -sy) + (sxz * cy), (sz * sy) + (csxz * cy), cx * cy);
}

void AR_VEC_CALL Matrix3::setRotationDeg(float32 x, float32 y, float32 z)
{
	setRotationRad(x * c_rads_per_deg, y * c_rads_per_deg, z * c_rads_per_deg);
}

void AR_VEC_CALL Matrix3::setRotation(const Quaternion &q)
{
	float32 s = q.getLengthSq();
	AR_ASSERT_MSG(s != 0.f, "Quaternion of length 0");
	s = 2.f / s;

#ifdef AR_SIMD
	__m128 row0, row1, row2;
	const __m128 qData = q.get128();
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

	m_rows[0].set(row0);
	m_rows[1].set(row1);
	m_rows[2].set(row2);
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

	set(
		1 - yy - zz, xy - wz, xz + wy,
		xy + wz, 1 - xx - zz, yz - wx,
		xz - wy, yz + wx, 1 - xx - yy
	);
#endif // ifdef AR_SIMD
}

void AR_VEC_CALL Matrix3::getQuaternion(Quaternion &q) const
{
	const float32 trace = m_rows[0][0] + m_rows[1][1] + m_rows[2][2];

	if (trace > 0)
	{
		const float32 s = std::sqrt(trace + 1.f) * 2.f;
		q.set(
			0.25f * s,
			(m_rows[2][1] - m_rows[1][2]) / s,
			(m_rows[0][2] - m_rows[2][0]) / s,
			(m_rows[1][0] - m_rows[0][1]) / s);
		return;
	}
	else if ((m_rows[0][0] > m_rows[1][1]) && (m_rows[0][0] > m_rows[2][2]))
	{
		const float32 s = std::sqrt(1.f + m_rows[0][0] - m_rows[1][1] - m_rows[2][2]) * 2;
		q.set(
			(m_rows[2][1] - m_rows[1][2]) / s,
			0.25f * s,
			(m_rows[0][1] + m_rows[1][0]) / s,
			(m_rows[0][2] + m_rows[2][0]) / s);
	}
	else if (m_rows[1][1] > m_rows[2][2])
	{
		const float32 s = std::sqrt(1.f + m_rows[1][1] - m_rows[0][0] - m_rows[2][2]) * 2;
		q.set(
			(m_rows[0][2] - m_rows[2][0]) / s,
			(m_rows[0][1] + m_rows[1][0]) / s,
			0.25f * s,
			(m_rows[1][2] + m_rows[2][1]) / s);
	}
	else
	{
		const float32 s = std::sqrt(1.f + m_rows[2][2] - m_rows[0][0] - m_rows[1][1]) * 2;
		q.set(
			(m_rows[1][0] - m_rows[0][1]) / s,
			(m_rows[0][2] + m_rows[2][0]) / s,
			(m_rows[1][2] + m_rows[2][1]) / s,
			0.25f * s);
	}
}

float32 AR_VEC_CALL Matrix3::getDeterminant(void) const
{
#ifdef AR_SIMD
	__m128 c1 = _mm_shuffle_ps(m_rows[1].get128(), m_rows[1].get128(), 0xC9);
	__m128 c2 = _mm_shuffle_ps(m_rows[2].get128(), m_rows[2].get128(), 0xC9); //_MM_SHUFFLE(3, 0, 2, 1)

	c1 = _mm_mul_ps(c1, m_rows[2].get128());
	c2 = _mm_mul_ps(c2, m_rows[1].get128());

	c1 = _mm_sub_ps(c2, c1);

	c1 = _mm_shuffle_ps(c1, c1, 0xC9);
	c1 = _mm_mul_ps(m_rows[0].get128(), c1);

	const __m128 z = _mm_movehl_ps(c1, c1);
	const __m128 y = _mm_shuffle_ps(c1, c1, 0xFD);  //_MM_SHUFFLE(3, 3, 3, 1)
	c1 = _mm_add_ps(z, _mm_add_ps(y, c1));

	return _mm_cvtss_f32(c1);
#else
	return m_rows[0].getX() * (m_rows[1].getY() * m_rows[2].getZ()
														 - m_rows[2].getY() * m_rows[1].getZ())
				 - m_rows[1].getX() * (m_rows[0].getY() * m_rows[2].getZ()
															 - m_rows[2].getY() * m_rows[0].getZ())
				 + m_rows[2].getX() * (m_rows[0].getY() * m_rows[1].getZ()
															 - m_rows[1].getY() * m_rows[0].getZ());
#endif // ifedf AR_SIMD
}

Matrix3 AR_VEC_CALL Matrix3::getTransposed(void) const
{
#ifdef AR_SIMD
	const __m128 temp = _mm_unpackhi_ps(m_rows[0].get128(), m_rows[1].get128());
	__m128 v0 = _mm_unpacklo_ps(m_rows[0].get128(), m_rows[1].get128());


	const __m128 v1 = _mm_shuffle_ps(v0, m_rows[2].get128(), 0xDE); //_MM_SHUFFLE(3, 1, 3, 2)
	const __m128 v2 = _mm_shuffle_ps(temp, m_rows[2].get128(), 0xE4); //_MM_SHUFFLE(3, 2, 1, 0)
	v0 = _mm_shuffle_ps(v0, m_rows[2].get128(), 0xC4); //_MM_SHUFFLE(3, 0, 1, 0)

	return Matrix3(v0, v1, v2);
#else
	return Matrix3(m_rows[0].getX(), m_rows[1].getX(), m_rows[2].getX(),
		m_rows[0].getY(), m_rows[1].getY(), m_rows[2].getY(),
		m_rows[0].getZ(), m_rows[1].getZ(), m_rows[2].getZ());
#endif // ifdef AR_SIMD
}

Matrix3& AR_VEC_CALL Matrix3::transpose(void)
{
	*this = getTransposed();

	return *this;
}

Matrix3 AR_VEC_CALL Matrix3::getCofactorMatrix(void) const
{
	return Matrix3(_cofactor(1, 1, 2, 2), -_cofactor(1, 0, 2, 2), _cofactor(1, 0, 2, 1),
		-_cofactor(0, 1, 2, 2), _cofactor(0, 0, 2, 2), -_cofactor(0, 0, 2, 1),
		_cofactor(0, 1, 1, 2), -_cofactor(0, 0, 1, 2), _cofactor(0, 0, 1, 1));
}

Matrix3 AR_VEC_CALL Matrix3::getAdjointMatrix(void) const
{
	return Matrix3(_cofactor(1, 1, 2, 2), -_cofactor(0, 1, 2, 2), _cofactor(0, 1, 1, 2),
		-_cofactor(1, 0, 2, 2), _cofactor(0, 0, 2, 2), -_cofactor(0, 0, 1, 2),
		_cofactor(1, 0, 2, 1), -_cofactor(0, 0, 2, 1), _cofactor(0, 0, 1, 1));
}

Matrix3 AR_VEC_CALL Matrix3::getInverted(void) const
{
	const float32 det = getDeterminant();

	AR_ASSERT_MSG(det != 0.f, "Determinant is zero");

	const float32 s = 1.f / det;

	return Matrix3(_cofactor(1, 1, 2, 2) * s, -_cofactor(0, 1, 2, 2) * s, _cofactor(0, 1, 1, 2) * s,
		-_cofactor(1, 0, 2, 2) * s, _cofactor(0, 0, 2, 2) * s, -_cofactor(0, 0, 1, 2) * s,
		_cofactor(1, 0, 2, 1) * s, -_cofactor(0, 0, 2, 1) * s, _cofactor(0, 0, 1, 1) * s);
}

Matrix3& AR_VEC_CALL Matrix3::invert(void)
{
	*this = getInverted();

	return *this;
}

Vector3 AR_VEC_CALL Matrix3::getColumn(int32 i) const
{
	AR_ASSERT_MSG(0 <= i && i < 3, "Index is out of range");

	return Vector3(m_rows[0][i], m_rows[1][i], m_rows[2][i]);
}

void AR_VEC_CALL Matrix3::setColumn(int32 i, const Vector3 &v)
{
	AR_ASSERT_MSG(0 <= i && i < 3, "Index is out of range");

	m_rows[0][i] = v[0];
	m_rows[1][i] = v[1];
	m_rows[2][i] = v[2];
}

Vector3& AR_VEC_CALL Matrix3::operator[](int32 i)
{
	AR_ASSERT_MSG(0 <= i && i < 3, "Index is out of range");

	return m_rows[i];
}

const Vector3& AR_VEC_CALL Matrix3::operator[](int32 i) const
{
	AR_ASSERT_MSG(0 <= i && i < 3, "Index is out of range");

	return m_rows[i];
}

Matrix3 AR_VEC_CALL Matrix3::operator+(const Matrix3 &other) const
{
	return Matrix3(m_rows[0] + other.m_rows[0], m_rows[1] + other.m_rows[1],
		m_rows[2] + other.m_rows[2]);
}

Matrix3 AR_VEC_CALL Matrix3::operator-(const Matrix3 &other) const
{
	return Matrix3(m_rows[0] - other.m_rows[0], m_rows[1] - other.m_rows[1],
		m_rows[2] - other.m_rows[2]);
}

Matrix3 AR_VEC_CALL Matrix3::operator*(const Matrix3 &other) const
{
#ifdef AR_SIMD
	const __m128 tv0 = m_rows[0].get128();
	const __m128 tv1 = m_rows[1].get128();
	const __m128 tv2 = m_rows[2].get128();
	const __m128 mv0 = other.m_rows[0].get128();
	const __m128 mv1 = other.m_rows[1].get128();
	const __m128 mv2 = other.m_rows[2].get128();

	__m128 v00 = _mm_shuffle_ps(tv0, tv0, 0x0);
	__m128 v01 = _mm_shuffle_ps(tv0, tv0, 0x55);
	__m128 v02 = _mm_shuffle_ps(tv0, tv0, 0xAA);

	v00 = _mm_mul_ps(v00, mv0);
	v01 = _mm_mul_ps(v01, mv1);
	v02 = _mm_mul_ps(v02, mv2);

	__m128 v10 = _mm_shuffle_ps(tv1, tv1, 0x0);
	__m128 v11 = _mm_shuffle_ps(tv1, tv1, 0x55);
	__m128 v12 = _mm_shuffle_ps(tv1, tv1, 0xAA);

	v10 = _mm_mul_ps(v10, mv0);
	v11 = _mm_mul_ps(v11, mv1);
	v12 = _mm_mul_ps(v12, mv2);

	__m128 v20 = _mm_shuffle_ps(tv2, tv2, 0x0);
	__m128 v21 = _mm_shuffle_ps(tv2, tv2, 0x55);
	__m128 v22 = _mm_shuffle_ps(tv2, tv2, 0xAA);

	v20 = _mm_mul_ps(v20, mv0);
	v21 = _mm_mul_ps(v21, mv1);
	v22 = _mm_mul_ps(v22, mv2);

	return Matrix3(_mm_add_ps(v00, _mm_add_ps(v01, v02)),
								 _mm_add_ps(v10, _mm_add_ps(v11, v12)),
								 _mm_add_ps(v20, _mm_add_ps(v21, v22)));
#else
	return Matrix3(
			m_rows[0].dot(other.getColumn(0)), m_rows[0].dot(other.getColumn(1)),
				m_rows[0].dot(other.getColumn(2)),
			m_rows[1].dot(other.getColumn(0)), m_rows[1].dot(other.getColumn(1)),
				m_rows[1].dot(other.getColumn(2)),
			m_rows[2].dot(other.getColumn(0)), m_rows[2].dot(other.getColumn(1)),
				m_rows[2].dot(other.getColumn(2)));
#endif // ifdef AR_SIMD
}

Matrix3& AR_VEC_CALL Matrix3::operator*=(const Matrix3 &other)
{
#ifdef AR_SIMD
	const __m128 tv0 = m_rows[0].get128();
	const __m128 tv1 = m_rows[1].get128();
	const __m128 tv2 = m_rows[2].get128();
	__m128 mv0 = other.m_rows[0].get128();
	__m128 mv1 = other.m_rows[1].get128();
	__m128 mv2 = other.m_rows[2].get128();

	__m128 v00 = _mm_shuffle_ps(tv0, tv0, 0x0);
	__m128 v01 = _mm_shuffle_ps(tv0, tv0, 0x55);
	__m128 v02 = _mm_shuffle_ps(tv0, tv0, 0xAA);

	v00 = _mm_mul_ps(v00, mv0);
	v01 = _mm_mul_ps(v01, mv1);
	v02 = _mm_mul_ps(v02, mv2);

	__m128 v10 = _mm_shuffle_ps(tv1, tv1, 0x0);
	__m128 v11 = _mm_shuffle_ps(tv1, tv1, 0x55);
	__m128 v12 = _mm_shuffle_ps(tv1, tv1, 0xAA);

	v10 = _mm_mul_ps(v10, mv0);
	v11 = _mm_mul_ps(v11, mv1);
	v12 = _mm_mul_ps(v12, mv2);

	__m128 v20 = _mm_shuffle_ps(tv2, tv2, 0x0);
	__m128 v21 = _mm_shuffle_ps(tv2, tv2, 0x55);
	__m128 v22 = _mm_shuffle_ps(tv2, tv2, 0xAA);

	v20 = _mm_mul_ps(v20, mv0);
	v21 = _mm_mul_ps(v21, mv1);
	v22 = _mm_mul_ps(v22, mv2);

	m_rows[0] = Vector3(_mm_add_ps(v00, _mm_add_ps(v01, v02)));
	m_rows[1] = Vector3(_mm_add_ps(v10, _mm_add_ps(v11, v12)));
	m_rows[2] = Vector3(_mm_add_ps(v20, _mm_add_ps(v21, v22)));
#else
	set(
		m_rows[0].dot(other.getColumn(0)), m_rows[0].dot(other.getColumn(1)),
			m_rows[0].dot(other.getColumn(2)),
		m_rows[1].dot(other.getColumn(0)), m_rows[1].dot(other.getColumn(1)),
			m_rows[1].dot(other.getColumn(2)),
		m_rows[2].dot(other.getColumn(0)), m_rows[2].dot(other.getColumn(1)),
			m_rows[2].dot(other.getColumn(2)));
#endif // ifdef AR_SIMD

	return *this;
}

Matrix3& AR_VEC_CALL Matrix3::operator+=(const Matrix3 &other)
{
	m_rows[0] += other.m_rows[0];
	m_rows[1] += other.m_rows[1];
	m_rows[2] += other.m_rows[2];

	return *this;
}

Matrix3& AR_VEC_CALL Matrix3::operator-=(const Matrix3 &other)
{
	m_rows[0] -= other.m_rows[0];
	m_rows[1] -= other.m_rows[1];
	m_rows[2] -= other.m_rows[2];

	return *this;
}

Matrix3 AR_VEC_CALL Matrix3::operator*(float32 s) const
{
	return Matrix3(m_rows[0] * s, m_rows[1] * s, m_rows[2] * s);
}

Vector3 AR_VEC_CALL Matrix3::operator*(const Vector3 &v) const
{
	return Vector3(m_rows[0].dot(v), m_rows[1].dot(v), m_rows[2].dot(v));
}

bool AR_VEC_CALL Matrix3::operator==(const Matrix3 &other) const
{
	return (m_rows[0] == other.m_rows[0]) && (m_rows[1] == other.m_rows[1])
		&& (m_rows[2] == other.m_rows[2]);
}

bool AR_VEC_CALL Matrix3::operator!=(const Matrix3 &other) const
{
	return !(*this == other);
}

float32 AR_VEC_CALL Matrix3::_cofactor(int32 r1, int32 c1, int32 r2, int32 c2) const
{
	return m_rows[r1][c1] * m_rows[r2][c2] - m_rows[r2][c1] * m_rows[r1][c2];
}
} // namespace Argon::Math
