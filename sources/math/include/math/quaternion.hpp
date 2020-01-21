#pragma once

#include <fundamental/compiler_macros.hpp>
#include <fundamental/types.hpp>

#include "matrix3.hpp"
#include "matrix4.hpp"
#include "simd.hpp"
#include "vector3.hpp"

namespace Argon::Math
{
AR_ATTR_ALIGN(16) class Quaternion final
{
public:
	Quaternion();
	Quaternion(const Quaternion &);
	explicit AR_VEC_CALL Quaternion(float32 q0, float32 q1, float32 q2, float32 q3);
	explicit AR_VEC_CALL Quaternion(const Vector3 &axis, float32 angleRad);
	explicit AR_VEC_CALL Quaternion(const Matrix3 &m);
	explicit AR_VEC_CALL Quaternion(const Matrix4 &m);
#ifdef AR_SIMD
	explicit AR_VEC_CALL Quaternion(__m128 vec);
	__m128 AR_VEC_CALL get128() const;
#endif // ifdef AR_SIMD

	Matrix3 AR_VEC_CALL getMatrix3() const;
	Matrix4 AR_VEC_CALL getMatrix4() const;
	void AR_VEC_CALL getEulerXYZ(float32 &roll, float32 &pitch, float32 &yaw) const;
	void AR_VEC_CALL getAxisAngle(Vector3 &axis, float32 &angle) const;

	void AR_VEC_CALL set(const Vector3 &axis, float32 angleRad);
	void AR_VEC_CALL set(const Matrix3 &m);
	void AR_VEC_CALL set(const Matrix4 &m);
	void AR_VEC_CALL set(float32 q0, float32 q1, float32 q2, float32 q3);
	//XZY
	void AR_VEC_CALL setEuler(float32 roll, float32 pitch, float32 yaw);

	Quaternion& AR_VEC_CALL invert(void);
	Quaternion AR_VEC_CALL getInverted(void) const;

	float32 AR_VEC_CALL dot(const Quaternion &other) const;

	float32 AR_VEC_CALL getLength(void) const;
	float32 AR_VEC_CALL getLengthSq(void) const;

	Quaternion AR_VEC_CALL getNormalized(void) const;
	Quaternion& AR_VEC_CALL normalize(void);

	Quaternion& AR_VEC_CALL operator=(const Quaternion &other);

	Quaternion AR_VEC_CALL operator+(const Quaternion &other) const;
	Quaternion& AR_VEC_CALL operator+=(const Quaternion &other);

	Quaternion AR_VEC_CALL operator-(const Quaternion &other) const;
	Quaternion& AR_VEC_CALL operator-=(const Quaternion &other);

	Quaternion AR_VEC_CALL operator*(float32 scalar) const;
	Quaternion& AR_VEC_CALL operator*=(float32 scalar);

	Quaternion AR_VEC_CALL operator/(float32 scalar) const;
	Quaternion& AR_VEC_CALL operator/=(float32 scalar);

	Quaternion AR_VEC_CALL operator*(const Quaternion &other) const;
	Quaternion& AR_VEC_CALL operator*=(const Quaternion &other);
	Quaternion AR_VEC_CALL operator*(const Vector3 &v) const;

	Quaternion AR_VEC_CALL slerp(const Quaternion &q, float32 t) const;

	Vector3 AR_VEC_CALL getRotated(const Vector3 &v) const;
	Vector3& AR_VEC_CALL rotate(Vector3 &v) const;

	float32& AR_VEC_CALL operator[](int32 i);
	const float32& AR_VEC_CALL operator[](int32 i) const;

private:
	//Scalar goes before the vector part
#ifdef AR_SIMD
	union
	{
		float32 m_comps[4];
		__m128 m_data;
	};
#else
	float32 m_comps[4];
#endif // ifdef AR_SIMD
};
} // namespace Argon::Math
