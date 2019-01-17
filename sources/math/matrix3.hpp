#pragma once

#include <fundamental/compiler_macros.hpp>
#include <fundamental/types.hpp>

#include "simd.hpp"
#include "vector3.hpp"

namespace Argon
{
class Quaternion;

AR_ATTR_ALIGN(16) class Matrix3 final
{
public:
	static const Matrix3 AR_ATTR_ALIGN(16) c_identity;

	Matrix3(void);
	AR_VEC_CALL Matrix3(const Matrix3 &other);
	explicit AR_VEC_CALL Matrix3(const Quaternion &q);
	explicit AR_VEC_CALL Matrix3(
		float32 e00, float32 e01, float32 e02,
		float32 e10, float32 e11, float32 e12,
		float32 e20, float32 e21, float32 e22);
	explicit AR_VEC_CALL Matrix3(const Vector3 &v0, const Vector3 &v1,
															 const Vector3 &v2);

#ifdef AR_SIMD
	AR_VEC_CALL Matrix3(__m128 v0, __m128 v1, __m128 v2);
#endif // ifdef AR_SIMD

	Matrix3& AR_VEC_CALL operator=(const Matrix3 &other);

	void AR_VEC_CALL Set(float32 e00, float32 e01, float32 e02,
											 float32 e10, float32 e11, float32 e12,
											 float32 e20, float32 e21, float32 e22);
	void SetIdentity();
	void ZeroOut(void);

	void AR_VEC_CALL SetRotationRad(float32 x, float32 y, float32 z);

	void AR_VEC_CALL SetRotationDeg(float32 x, float32 y, float32 z);

	void AR_VEC_CALL SetRotation(const Quaternion &q);

	void AR_VEC_CALL GetQuaternion(Quaternion &q) const;

	float32 AR_VEC_CALL Determinant(void) const;

	Matrix3 AR_VEC_CALL Transposed(void) const;

	Matrix3& AR_VEC_CALL Transpose(void);

	Matrix3 AR_VEC_CALL GetCofactorMatrix(void) const;

	Matrix3 AR_VEC_CALL GetAdjointMatrix(void) const;

	Matrix3 AR_VEC_CALL Inverted(void) const;

	Matrix3& AR_VEC_CALL Invert(void);

	Vector3 AR_VEC_CALL GetColumn(int32 i) const;

	void AR_VEC_CALL SetColumn(int32 i, const Vector3 &v);

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
	float32 AR_VEC_CALL _Cofactor(int32 r1, int32 c1, int32 r2, int32 c2) const;

	Vector3 m_rows[3];
};
} // namespace Argon
