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
template <typename ...Args>
void ErrorMsg(Args &&...args)
{
	static_assert(sizeof...(args), "ErrorMsg should be called with at least one argument");

	std::cerr << "ERROR MSG: ";
	(std::cerr << ... << args) << std::endl;
}

template <typename ...Args>
void StatusMsg(Args &&...args)
{
	static_assert(sizeof...(args), "StatusMsg should be called with at least one argument");

	std::cerr << "STATUS MSG: ";
	(std::cerr << ... << args) << std::endl;
}
} // namespace Debug
} // namespace Argon
