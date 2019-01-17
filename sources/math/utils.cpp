#include <cmath>

#include <fundamental/compiler_macros.hpp>
#include <fundamental/debug.hpp>
#include <fundamental/types.hpp>

#include "utils.hpp"
#include "vector3.hpp"

namespace Argon
{
namespace MathUtils
{
float32 Round(float32 val, uint16 numDecimals)
{
	AR_ASSERT_MSG(numDecimals != 0, "Rounding error");

	const auto mul = static_cast<int32>(std::pow(10, numDecimals));

	return std::roundf(val * mul) / mul;
}

void GramSchmidt(Vector3 &e1, Vector3 &e2, Vector3 &e3)
{
	const float32 absX = std::abs(e1[0]);
	if (absX > std::abs(e1[1]) && absX > std::abs(e1[2]))
	{
		e2[0] = -e1[1];
		e2[1] = e1[0];
		e2[2] = 0.f;
	}
	else
	{
		e2[0] = 0.f;
		e2[1] = e1[2];
		e2[2] = -e1[1];
	}

	e1.Normalize();
	e2.Normalize();
	e3 = e1.Cross(e2);
	e3.Normalize();

	AR_ASSERT_MSG(e1.Dot(e2) == 0.f, "Basis is not orthogonal");
	AR_ASSERT_MSG(e1.Dot(e3) == 0.f, "Basis is not orthogonal");
}
} // namespace MathUtils
} // namespace Argon
