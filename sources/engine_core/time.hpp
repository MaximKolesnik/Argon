#pragma once

#include <memory>

#include <fundamental/compiler_macros.hpp>
#include <fundamental/helper_macros.hpp>
#include <fundamental/types.hpp>

#include "core_singleton.hpp"

namespace Argon
{
class AR_SYM_EXPORT Time final
	: public CoreSingleton
{
public:
	Time();
	~Time();

	float32 GetDelta() const;
	float32 GetUnscaledDelta() const;

	void SetDeltaScale(float32 val);
	float32 GetDeltaScale() const;

	void EndFrame();

private:
	AR_PRIVATE_IMPL(Time);
};
} // namespace Argon
