#pragma once

#include <fundamental/compiler_macros.hpp>
#include <fundamental/types.hpp>

#include "simd.hpp"
#include "vector3.hpp"
#include "vector4.hpp"

namespace Argon::Math
{
class Quaternion;

AR_ATTR_ALIGN(16) class __attribute__ ((visibility ("default"))) Matrix4 final
{
public:
	static const Matrix4 AR_ATTR_ALIGN(16) c_identity;

	Matrix4();
	AR_VEC_CALL Matrix4(const Matrix4 &other);
	explicit AR_VEC_CALL Matrix4(const Quaternion &q);
	explicit AR_VEC_CALL Matrix4(
		float32 e00, float32 e01, float32 e02, float32 e03,
		float32 e10, float32 e11, float32 e12, float32 e13,
		float32 e20, float32 e21, float32 e22, float32 e23,
		float32 e30, float32 e31, float32 e32, float32 e33);
	explicit AR_VEC_CALL Matrix4(const Vector4 &v0, const Vector4 &v1,
															 const Vector4 &v2, const Vector4 &v3);
	// Will set e33 to 1.0
	explicit AR_VEC_CALL Matrix4(const Vector3 &v0, const Vector3 &v1,
															 const Vector3 &v2, const Vector3 &v3);

#ifdef AR_SIMD
	explicit AR_VEC_CALL Matrix4(__m128 v0, __m128 v1, __m128 v2, __m128 v3);
#endif // ifdef AR_SIMD

	Matrix4& AR_VEC_CALL operator=(const Matrix4 &other);

	void AR_VEC_CALL set(
		float32 e00, float32 e01, float32 e02, float32 e03,
		float32 e10, float32 e11, float32 e12, float32 e13,
		float32 e20, float32 e21, float32 e22, float32 e23,
		float32 e30, float32 e31, float32 e32, float32 e33);
	void setIdentity(void);

	void zeroOut(void);

	//XZY
	void AR_VEC_CALL setRotationRad(float32 roll, float32 pitch, float32 yaw);
	void AR_VEC_CALL setScaling(float32 x, float32 y, float32 z);
	void AR_VEC_CALL setTranslation(float32 x, float32 y, float32 z);

	//XZY
	void AR_VEC_CALL setTransformation(const Vector3 &euler,
		const Vector3 &scaling, const Vector3 &trans);
	void AR_VEC_CALL setTransformation(const Quaternion &rotation,
		const Vector3 &scaling, const Vector3 &trans);

	void AR_VEC_CALL setRotationDeg(float32 roll, float32 pitch, float32 yaw);
	void AR_VEC_CALL setRotation(const Quaternion &q);

	void AR_VEC_CALL getQuaternion(Quaternion &q) const;

	void AR_VEC_CALL setLookAtLH(const Vector3& eye, const Vector3& lookAt,
															 const Vector3& up);
	void AR_VEC_CALL setViewMatrix(const Vector3& right, const Vector3& up,
																 const Vector3& forward, const Vector3& eye);
	void AR_VEC_CALL setOrthographicLH(float32 width, float32 height, float32 nearPlane,
																		 float32 farPlane);
	void AR_VEC_CALL setPerspectiveFovLH(float32 width, float32 height, float32 fov,
																			 float32 nearPlane, float32 farPlane);

	// Calculate determinant, expanding the first column
	// Use short determinant, if the matrix is a transformation matrix
	float32 AR_VEC_CALL getDeterminantFull(void) const;
	//Assumes the matrix is an affine transformation matrix
	float32 AR_VEC_CALL getDeterminantFast(void) const;

	Matrix4 AR_VEC_CALL getTransposed(void) const;
	Matrix4& AR_VEC_CALL transpose(void);

	//Provided by intel
	//download.intel.com/design/pentiumiii/sml/24504301.pdf
	Matrix4 AR_VEC_CALL getCofactorMatrix(void) const;
	Matrix4 AR_VEC_CALL getAdjointMatrix(void) const;

	Matrix4 AR_VEC_CALL getInverted(void) const;
	Matrix4& AR_VEC_CALL invert(void);

	Vector3 AR_VEC_CALL transformNormal(const Vector3 &n) const;

	Vector3 AR_VEC_CALL getColumn(int32 i) const;
	void AR_VEC_CALL setColumn(int32 i, const Vector4 &v);

	Vector4& AR_VEC_CALL operator[](int32 i);
	const Vector4& AR_VEC_CALL operator[](int32 i) const;

	Matrix4 AR_VEC_CALL operator+(const Matrix4 &other) const;
	Matrix4& AR_VEC_CALL operator+=(const Matrix4 &other);

	Matrix4 AR_VEC_CALL operator-(const Matrix4 &other) const;
	Matrix4& AR_VEC_CALL operator-=(const Matrix4 &other);

	Matrix4 AR_VEC_CALL operator*(const Matrix4 &other) const;
	Matrix4& AR_VEC_CALL operator*=(const Matrix4 &other);
	Matrix4 AR_VEC_CALL operator*(float32 s) const;
	Vector3 AR_VEC_CALL operator*(const Vector3 &v) const;
	Vector4 AR_VEC_CALL operator*(const Vector4 &v) const;

	bool AR_VEC_CALL operator==(const Matrix4 &other) const;
	bool AR_VEC_CALL operator!=(const Matrix4 &other) const;

private:
	Vector4 AR_VEC_CALL _getColumn4(int32 i) const;

	Vector4 m_rows[4];
};
} // namespace Argon::Math
