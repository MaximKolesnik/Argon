#pragma once

#include <memory>

#include <fundamental/compiler_macros.hpp>
#include <fundamental/helper_macros.hpp>
#include <fundamental/types.hpp>

namespace Argon::Core
{
class AR_SYM_EXPORT Time final
{
public:
	Time();
	~Time();

	float32 getDelta() const;
	float32 getUnscaledDelta() const;

	void setDeltaScale(float32 val);
	float32 getDeltaScale() const;

	void endFrame();

private:
	AR_PRIVATE_IMPL(Time);
};
} // namespace Argon::Core
