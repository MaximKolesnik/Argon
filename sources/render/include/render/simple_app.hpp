#pragma once

#include <fundamental/compiler_macros.hpp>

namespace argon
{
class AR_SYM_EXPORT SimpleApp
{
public:
	void prepare(const char *vShader, const char *fShader);
	bool shouldClose();
	void tick();
	void close();
};
}
