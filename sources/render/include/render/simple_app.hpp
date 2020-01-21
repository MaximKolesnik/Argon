#pragma once

#include <fundamental/compiler_macros.hpp>

namespace Argon
{
class AR_SYM_EXPORT SimpleApp
{
public:
	void prepare();
	bool shouldClose();
	void tick();
	void close();
};
}
