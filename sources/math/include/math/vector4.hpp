#pragma once

#include <fundamental/compiler_macros.hpp>
#include <fundamental/types.hpp>

#include "forward_declarations.hpp"
#include "simd.hpp"

namespace argon::math
{
AR_ATTR_ALIGN(16) class Vector4 final
{
public:
	static const Vector4 AR_ATTR_ALIGN(16) c_zero;
	static const Vector4 AR_ATTR_ALIGN(16) c_xAxis;
	static const Vector4 AR_ATTR_ALIGN(16) c_yAxis;
	static const Vector4 AR_ATTR_ALIGN(16) c_zAxis;
	static const Vector4 AR_ATTR_ALIGN(16) c_wAxis;

	Vector4();
	AR_VEC_CALL Vector4(const Vector4 &);
	explicit AR_VEC_CALL Vector4(float32 x, float32 y, float32 z, float32 w);

	Vector4& AR_VEC_CALL operator=(const Vector4 &other);

#ifdef AR_SIMD
	explicit AR_VEC_CALL Vector4(__m128 data);
	__m128 AR_VEC_CALL get128() const;
	void AR_VEC_CALL set(__m128 data);
#endif // ifdef AR_SIMD

	float32 AR_VEC_CALL getX() const;
	float32 AR_VEC_CALL getY() const;
	float32 AR_VEC_CALL getZ() const;
	float32 AR_VEC_CALL getW() const;

	void AR_VEC_CALL setX(float32 x);
	void AR_VEC_CALL setY(float32 y);
	void AR_VEC_CALL setZ(float32 z);
	void AR_VEC_CALL setW(float32 w);
	void AR_VEC_CALL set(float32 x, float32 y, float32 z, float32 w);

	void zeroOut(void);

	float32 AR_VEC_CALL dot(const Vector4 &other) const;
	//dot with vector3 as if w component is 1
	float32 AR_VEC_CALL dot(const Vector3 &other) const;
	//dot with vector3 treating w component as 0
	float32 AR_VEC_CALL dot3(const Vector3 &other) const;

	float32 AR_VEC_CALL getLength(void) const;
	float32 AR_VEC_CALL getLengthSq(void) const;

	Vector4& AR_VEC_CALL normalize(void);
	Vector4 AR_VEC_CALL getNormalized(void) const;

	bool isZeroEpsilon(void) const;

	Vector4 AR_VEC_CALL operator+(const Vector4 &other) const;
	Vector4& AR_VEC_CALL operator+=(const Vector4 &other);

	Vector4 AR_VEC_CALL operator-(void) const;
	Vector4 AR_VEC_CALL operator-(const Vector4 &other) const;
	Vector4& AR_VEC_CALL operator-=(const Vector4 &other);

	Vector4 AR_VEC_CALL operator*(float32 scalar) const;
	Vector4& AR_VEC_CALL operator*=(float32 scalar);

	Vector4 AR_VEC_CALL operator/(float32 scalar) const;
	Vector4& AR_VEC_CALL operator/=(float32 scalar);

	float32& AR_VEC_CALL operator[](int32 i);
	const float32& AR_VEC_CALL operator[](int32 i) const;

	bool AR_VEC_CALL operator==(const Vector4 &other) const;
	bool AR_VEC_CALL operator!=(const Vector4 &other) const;
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

Vector4 AR_VEC_CALL operator*(float32 scalar, const Vector4 &v);
} // namespace argon::math
