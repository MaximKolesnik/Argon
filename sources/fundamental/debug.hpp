#pragma once

#include <cassert>
#include <iostream>

//TODO fix AR_ASSERT_MSG

#define AR_ASSERT(EXPR) assert(EXPR)
#define AR_ASSERT_MSG(EXPR, MSG) assert(EXPR)

namespace Argon
{
namespace Debug
{
template <typename FirstArg, typename ...Args>
void ErrorMsg(const FirstArg &arg, Args &&...args)
{
	std::cerr << arg;
	if constexpr (sizeof...(Args) == 0)
		std::cerr << std::endl;
	else
		ErrorMsg(std::forward<Args>(args)...);
}

template <typename FirstArg, typename ...Args>
void InfoMsg(const FirstArg &arg, Args &&...args)
{
	std::cout << arg;
	if constexpr (sizeof...(Args) == 0)
		std::cout << std::endl;
	else
		InfoMsg(std::forward<Args>(args)...);
}
} // namespace Debug
} // namespace Argon
