#pragma once

#include <fundamental/compiler_macros.hpp>
#include <fundamental/types.hpp>

#include "simd.hpp"

namespace Argon
{
class Vector3;

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
	__m128 AR_VEC_CALL Get128() const;
	void AR_VEC_CALL Set(__m128 data);
#endif // ifdef AR_SIMD

	float32 AR_VEC_CALL GetX() const;
	float32 AR_VEC_CALL GetY() const;
	float32 AR_VEC_CALL GetZ() const;
	float32 AR_VEC_CALL GetW() const;

	void AR_VEC_CALL SetX(float32 x);
	void AR_VEC_CALL SetY(float32 y);
	void AR_VEC_CALL SetZ(float32 z);
	void AR_VEC_CALL SetW(float32 w);
	void AR_VEC_CALL Set(float32 x, float32 y, float32 z, float32 w);

	void ZeroOut(void);

	float32 AR_VEC_CALL Dot(const Vector4 &other) const;
	//dot with vector3 as if w component is 1
	float32 AR_VEC_CALL Dot(const Vector3 &other) const;
	//dot with vector3 treating w component as 0
	float32 AR_VEC_CALL Dot3(const Vector3 &other) const;

	float32 AR_VEC_CALL Length(void) const;
	float32 AR_VEC_CALL LengthSq(void) const;

	Vector4& AR_VEC_CALL Normalize(void);
	Vector4 AR_VEC_CALL Normalized(void) const;

	bool IsZeroEpsilon(void) const;

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
} // namespace Argon
