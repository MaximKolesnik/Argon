#pragma once

#include <fundamental/compiler_macros.hpp>
#include <fundamental/types.hpp>

#include "simd.hpp"

namespace Argon::Math
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
	__m128 AR_VEC_CALL get128() const;
	void AR_VEC_CALL set(__m128 data);
#endif // ifdef AR_SIMD

	float32 AR_VEC_CALL getX() const;
	float32 AR_VEC_CALL getY() const;
	float32 AR_VEC_CALL getZ() const;

	void AR_VEC_CALL setX(float32 x);
	void AR_VEC_CALL setY(float32 y);
	void AR_VEC_CALL setZ(float32 z);
	void AR_VEC_CALL set(float32 x, float32 y, float32 z);

	void AR_VEC_CALL splat(float32 xyz);

	float32 AR_VEC_CALL dot(const Vector3 &other) const;
	Vector3 AR_VEC_CALL cross(const Vector3 &other) const;

	float32 AR_VEC_CALL getLength() const;
	float32 AR_VEC_CALL getLengthSq() const;

	Vector3& AR_VEC_CALL normalize();
	Vector3 AR_VEC_CALL getNormalized() const;

	void zeroOut();
	bool isZeroEpsilon() const;

	//Check if any of components has infinity
	bool hasInfinity() const;

	//Return a vector with absolute values of each component
	Vector3 AR_VEC_CALL getAbs(void) const;
	//Performs abs operation on the current vector
	Vector3& AR_VEC_CALL abs(void);

	Vector3 AR_VEC_CALL getNegated(void) const;
	Vector3& AR_VEC_CALL negate(void);

	float32 AR_VEC_CALL getDistance(const Vector3 &other) const;
	float32 AR_VEC_CALL getDistanceSq(const Vector3 &other) const;

	//Return the index of the max axis
	int32 AR_VEC_CALL getMaxAxis(void) const;
	//Return the index of the min axis
	int32 AR_VEC_CALL getMinAxis(void) const;
	//Return the value of the max axis
	float32 AR_VEC_CALL getMaxAxisValue(void) const;
	//Return the value of the min axis
	float32 AR_VEC_CALL getMinAxisValue(void) const;

	Vector3 AR_VEC_CALL lerp(const Vector3 &other, float32 t) const;

	//Get rotated vector around arbitrary axis. Axis has to be normalized
	Vector3 AR_VEC_CALL getRotatedRad(const Vector3 &axis, float32 angleRad) const;
	Vector3& AR_VEC_CALL rotateRad(const Vector3 &axis, float32 angleRad);

	Vector3 AR_VEC_CALL getRotatedDeg(const Vector3 &axis, float32 angleDeg) const;
	Vector3& AR_VEC_CALL rotateDeg(const Vector3 &axis, float32 angleDeg);

	float32 AR_VEC_CALL getAngle(const Vector3 &other) const;

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

	void AR_VEC_CALL round(uint16 numDecimals);
	Vector3 AR_VEC_CALL getRounded(uint16 numDecimals) const;

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
} // namespace Argon::Math
