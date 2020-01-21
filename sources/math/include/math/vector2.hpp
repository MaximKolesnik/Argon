#pragma once

#include <fundamental/compiler_macros.hpp>
#include <fundamental/types.hpp>

#include "simd.hpp"

namespace Argon::Math
{
AR_ATTR_ALIGN(16) class Vector2 final
{
public:
	static const Vector2 AR_ATTR_ALIGN(16) c_zero;
	static const Vector2 AR_ATTR_ALIGN(16) c_xAxis;
	static const Vector2 AR_ATTR_ALIGN(16) c_yAxis;

	Vector2();
	AR_VEC_CALL Vector2(const Vector2 &);
	explicit AR_VEC_CALL Vector2(float32 x, float32 y);

#ifdef AR_SIMD
	explicit AR_VEC_CALL Vector2(__m128 data);
#endif // ifdef AR_SIMD

	float32 AR_VEC_CALL getX(void) const;
	float32 AR_VEC_CALL getY(void) const;

	void AR_VEC_CALL setX(float32 x);
	void AR_VEC_CALL setY(float32 y);
	void AR_VEC_CALL set(float32 x, float32 y);

	float32 AR_VEC_CALL dot(const Vector2 &other) const;

	float32 AR_VEC_CALL getLength(void) const;
	float32 AR_VEC_CALL getLengthSq(void) const;

	Vector2& AR_VEC_CALL normalize(void);
	Vector2 AR_VEC_CALL getNormalized(void) const;

	void zeroOut(void);
	bool isZero(void) const;
	bool isZeroEpsilon(void) const;

	//Check if any of components has infinity
	bool hasInfinity(void) const;

	//Return a vector with absolute values of each component
	Vector2 AR_VEC_CALL getAbs(void) const;
	//Performs abs operation on the current vector
	Vector2& AR_VEC_CALL abs(void);

	Vector2 AR_VEC_CALL getNegated(void) const;
	Vector2& AR_VEC_CALL negate(void);

	float32 AR_VEC_CALL getDistance(const Vector2 &other) const;
	float32 AR_VEC_CALL getDistanceSq(const Vector2 &other) const;

	//Return the index of the max axis
	int32 AR_VEC_CALL getMaxAxis(void) const;
	//Return the index of the min axis
	int32 AR_VEC_CALL getMinAxis(void) const;
	//Return the value of the max axis
	float32 AR_VEC_CALL getMaxAxisValue(void) const;
	//Return the value of the min axis
	float32 AR_VEC_CALL getMinAxisValue(void) const;

	Vector2 AR_VEC_CALL lerp(const Vector2 &other, float32 t) const;

	Vector2 AR_VEC_CALL getRotatedRad(float32 angleRad) const;
	Vector2& AR_VEC_CALL rotateRad(float32 angleRad);

	Vector2 AR_VEC_CALL getRotatedDeg(float32 angleDeg) const;
	Vector2& AR_VEC_CALL rotateDeg(float32 angleDeg);

	float32 AR_VEC_CALL getAngle(const Vector2 &other) const;

	Vector2& AR_VEC_CALL operator=(const Vector2 &other);

	Vector2 AR_VEC_CALL operator+(const Vector2 &other) const;
	Vector2& AR_VEC_CALL operator+=(const Vector2 &other);

	Vector2 AR_VEC_CALL operator-(void) const;
	Vector2 AR_VEC_CALL operator-(const Vector2 &other) const;
	Vector2& AR_VEC_CALL operator-=(const Vector2 &other);

	Vector2 AR_VEC_CALL operator*(float32 scalar) const;
	Vector2& AR_VEC_CALL operator*=(float32 scalar);

	Vector2 AR_VEC_CALL operator/(float32 scalar) const;
	Vector2& AR_VEC_CALL operator/=(float32 scalar);

	//Index access
	float32& AR_VEC_CALL operator[](int32 i);
	const float32& AR_VEC_CALL operator[](int32 i) const;

	bool AR_VEC_CALL operator==(const Vector2 &other) const;
	bool AR_VEC_CALL operator!=(const Vector2 &other) const;

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

Vector2 AR_VEC_CALL operator*(float32 s, const Vector2 &v);
} // namespace Argon::Math
