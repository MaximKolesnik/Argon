#pragma once

#include <fundamental/compiler_macros.hpp>
#include <fundamental/types.hpp>

#include "simd.hpp"

namespace Argon
{
AR_ATTR_ALIGN(16) class Vector3 final
{
public:
	static const Vector3 AR_ATTR_ALIGN(16) c_zero;
	static const Vector3 AR_ATTR_ALIGN(16) c_xAxis;
	static const Vector3 AR_ATTR_ALIGN(16) c_yAxis;
	static const Vector3 AR_ATTR_ALIGN(16) c_zAxis;

	Vector3();
	AR_VEC_CALL Vector3(const Vector3 &);
	explicit AR_VEC_CALL Vector3(float32 x, float32 y, float32 z);

#ifdef AR_SIMD
	explicit AR_VEC_CALL Vector3(__m128 data);
	__m128 AR_VEC_CALL Get128() const;
	void AR_VEC_CALL Set(__m128 data);
#endif // ifdef AR_SIMD

	float32 AR_VEC_CALL GetX() const;
	float32 AR_VEC_CALL GetY() const;
	float32 AR_VEC_CALL GetZ() const;

	void AR_VEC_CALL SetX(float32 x);
	void AR_VEC_CALL SetY(float32 y);
	void AR_VEC_CALL SetZ(float32 z);
	void AR_VEC_CALL Set(float32 x, float32 y, float32 z);

	void AR_VEC_CALL Splat(float32 xyz);

	float32 AR_VEC_CALL Dot(const Vector3 &other) const;
	Vector3 AR_VEC_CALL Cross(const Vector3 &other) const;

	float32 AR_VEC_CALL Length() const;
	float32 AR_VEC_CALL LengthSq() const;

	Vector3& AR_VEC_CALL Normalize();
	Vector3 AR_VEC_CALL Normalized() const;

	void ZeroOut();
	bool IsZeroEpsilon() const;

	//Check if any of components has infinity
	bool HasInfinity() const;

	//Return a vector with absolute values of each component
	Vector3 AR_VEC_CALL GetAbs(void) const;
	//Performs abs operation on the current vector
	Vector3& AR_VEC_CALL Abs(void);

	//Returns a negated vector
	Vector3 AR_VEC_CALL Negated(void) const;
	//Negates the current vector
	Vector3& AR_VEC_CALL Negate(void);

	//Return distance between two points
	float32 AR_VEC_CALL Distance(const Vector3 &other) const;
	float32 AR_VEC_CALL DistanceSq(const Vector3 &other) const;

	//Return the index of the max axis
	int32 AR_VEC_CALL MaxAxis(void) const;
	//Return the index of the min axis
	int32 AR_VEC_CALL MinAxis(void) const;
	//Return the value of the max axis
	float32 AR_VEC_CALL MaxAxisValue(void) const;
	//Return the value of the min axis
	float32 AR_VEC_CALL MinAxisValue(void) const;

	//Linear interpolation
	Vector3 AR_VEC_CALL Lerp(const Vector3 &other, float32 t) const;

	//Get rotated vector around arbitrary axis. Axis has to be normalized
	Vector3 AR_VEC_CALL RotatedRad(const Vector3 &axis, float32 angleRad) const;
	Vector3& AR_VEC_CALL RotateRad(const Vector3 &axis, float32 angleRad);

	Vector3 AR_VEC_CALL RotatedDeg(const Vector3 &axis, float32 angleDeg) const;
	Vector3& AR_VEC_CALL RotateDeg(const Vector3 &axis, float32 angleDeg);

	//Get angle between two vectors
	float32 AR_VEC_CALL GetAngle(const Vector3 &other) const;

	Vector3& AR_VEC_CALL operator=(const Vector3 &other);

	Vector3 AR_VEC_CALL operator+(const Vector3 &other) const;
	Vector3& AR_VEC_CALL operator+=(const Vector3 &other);

	Vector3 AR_VEC_CALL operator-(void) const;
	Vector3 AR_VEC_CALL operator-(const Vector3 &other) const;
	Vector3& AR_VEC_CALL operator-=(const Vector3 &other);

	Vector3 AR_VEC_CALL operator*(float32 scalar) const;
	Vector3& AR_VEC_CALL operator*=(float32 scalar);
	Vector3 AR_VEC_CALL operator*(const Vector3 &other) const;

	Vector3 AR_VEC_CALL operator/(const Vector3 &other) const;
	Vector3 AR_VEC_CALL operator/(float32 scalar) const;
	Vector3& AR_VEC_CALL operator/=(float32 scalar);

	float32& AR_VEC_CALL operator[](int32 i);
	const float32& AR_VEC_CALL operator[](int32 i) const;

	bool AR_VEC_CALL operator==(const Vector3 &other) const;
	bool AR_VEC_CALL operator!=(const Vector3 &other) const;

	void AR_VEC_CALL Round(uint16 numDecimals);
	Vector3 AR_VEC_CALL Rounded(uint16 numDecimals) const;

private:
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

Vector3 AR_VEC_CALL operator*(float32 scalar, const Vector3 &v);
} // namespace Argon
