#pragma once

#include <fundamental/compiler_macros.hpp>
#include <fundamental/types.hpp>

#include "simd.hpp"

namespace Argon
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

	float32 AR_VEC_CALL GetX(void) const;
	float32 AR_VEC_CALL GetY(void) const;

	void AR_VEC_CALL SetX(float32 x);
	void AR_VEC_CALL SetY(float32 y);
	void AR_VEC_CALL Set(float32 x, float32 y);

	float32 AR_VEC_CALL Dot(const Vector2 &other) const;

	float32 AR_VEC_CALL Length(void) const;
	float32 AR_VEC_CALL LengthSq(void) const;

	Vector2& AR_VEC_CALL Normalize(void);
	Vector2 AR_VEC_CALL Normalized(void) const;

	void ZeroOut(void);
	bool IsZero(void) const;
	bool IsZeroEpsilon(void) const;

	//Check if any of components has infinity
	bool HasInfinity(void) const;

	//Return a vector with absolute values of each component
	Vector2 AR_VEC_CALL GetAbs(void) const;
	//Performs abs operation on the current vector
	Vector2& AR_VEC_CALL Abs(void);

	Vector2 AR_VEC_CALL Negated(void) const;
	Vector2& AR_VEC_CALL Negate(void);

	float32 AR_VEC_CALL Distance(const Vector2 &other) const;
	float32 AR_VEC_CALL DistanceSq(const Vector2 &other) const;

	//Return the index of the max axis
	int32 AR_VEC_CALL MaxAxis(void) const;
	//Return the index of the min axis
	int32 AR_VEC_CALL MinAxis(void) const;
	//Return the value of the max axis
	float32 AR_VEC_CALL MaxAxisValue(void) const;
	//Return the value of the min axis
	float32 AR_VEC_CALL MinAxisValue(void) const;

	//Linear interpolation
	Vector2 AR_VEC_CALL Lerp(const Vector2 &other, float32 t) const;

	//Get rotated vector
	Vector2 AR_VEC_CALL RotatedRad(float32 angleRad) const;
	//Rotate current vector
	Vector2& AR_VEC_CALL RotateRad(float32 angleRad);

	Vector2 AR_VEC_CALL RotatedDeg(float32 angleDeg) const;
	Vector2& AR_VEC_CALL RotateDeg(float32 angleDeg);

	//Get angle between two vectors
	float32 AR_VEC_CALL GetAngle(const Vector2 &other) const;

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
} // namespace Argon
