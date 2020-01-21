#pragma once

#include <fundamental/compiler_macros.hpp>
#include <fundamental/types.hpp>

#include "simd.hpp"
#include "vector3.hpp"

namespace Argon::Math
{
class Quaternion;

AR_ATTR_ALIGN(16) class Matrix3 final
{
public:
	static const Matrix3 AR_ATTR_ALIGN(16) c_identity;

	Matrix3(void);
	AR_VEC_CALL Matrix3(const Matrix3 &other);
	explicit AR_VEC_CALL Matrix3(const Quaternion &q);
	explicit AR_VEC_CALL Matrix3(float32 e00, float32 e01, float32 e02,
															 float32 e10, float32 e11, float32 e12,
															 float32 e20, float32 e21, float32 e22);
	explicit AR_VEC_CALL Matrix3(const Vector3 &v0, const Vector3 &v1,
															 const Vector3 &v2);

#ifdef AR_SIMD
	AR_VEC_CALL Matrix3(__m128 v0, __m128 v1, __m128 v2);
#endif // ifdef AR_SIMD

	Matrix3& AR_VEC_CALL operator=(const Matrix3 &other);

	void AR_VEC_CALL set(float32 e00, float32 e01, float32 e02,
											 float32 e10, float32 e11, float32 e12,
											 float32 e20, float32 e21, float32 e22);
	void setIdentity();
	void zeroOut(void);

	void AR_VEC_CALL setRotationRad(float32 x, float32 y, float32 z);

	void AR_VEC_CALL setRotationDeg(float32 x, float32 y, float32 z);

	void AR_VEC_CALL setRotation(const Quaternion &q);

	void AR_VEC_CALL getQuaternion(Quaternion &q) const;

	float32 AR_VEC_CALL getDeterminant(void) const;

	Matrix3 AR_VEC_CALL getTransposed(void) const;

	Matrix3& AR_VEC_CALL transpose(void);

	Matrix3 AR_VEC_CALL getCofactorMatrix(void) const;

	Matrix3 AR_VEC_CALL getAdjointMatrix(void) const;

	Matrix3 AR_VEC_CALL getInverted(void) const;

	Matrix3& AR_VEC_CALL invert(void);

	Vector3 AR_VEC_CALL getColumn(int32 i) const;

	void AR_VEC_CALL setColumn(int32 i, const Vector3 &v);

	Vector3& AR_VEC_CALL operator[](int32 i);

	const Vector3& AR_VEC_CALL operator[](int32 i) const;

	Matrix3 AR_VEC_CALL operator+(const Matrix3 &other) const;

	Matrix3 AR_VEC_CALL operator-(const Matrix3 &other) const;

	Matrix3 AR_VEC_CALL operator*(const Matrix3 &other) const;

	Matrix3& AR_VEC_CALL operator*=(const Matrix3 &other);

	Matrix3& AR_VEC_CALL operator+=(const Matrix3 &other);

	Matrix3& AR_VEC_CALL operator-=(const Matrix3 &other);

	Matrix3 AR_VEC_CALL operator*(float32 s) const;

	Vector3 AR_VEC_CALL operator*(const Vector3 &v) const;

	bool AR_VEC_CALL operator==(const Matrix3 &other) const;

	bool AR_VEC_CALL operator!=(const Matrix3 &other) const;

private:
	float32 AR_VEC_CALL _cofactor(int32 r1, int32 c1, int32 r2, int32 c2) const;

	Vector3 m_rows[3];
};
} // namespace Argon::Math
