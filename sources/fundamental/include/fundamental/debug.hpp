#pragma once

#include <cassert>
#include <iostream>

// TODO fix AR_ASSERT_MSG
// TODO fix AR_CRITICAL

#define AR_ASSERT(EXPR) assert(EXPR)
#define AR_ASSERT_MSG(EXPR, MSG) assert(EXPR)
#define AR_CRITICAL(EXPR, MSG) AR_ASSERT_MSG(EXPR, MSG)

namespace argon::debug
{
template <typename ...Args>
void errorMsg(Args &&...args)
{
	static_assert(sizeof...(args), "ErrorMsg should be called with at least one argument");

	std::cerr << "ERROR MSG: ";
	(std::cerr << ... << args) << std::endl;
}

template <typename ...Args>
void statusMsg(Args &&...args)
{
	static_assert(sizeof...(args), "StatusMsg should be called with at least one argument");

	std::cerr << "STATUS MSG: ";
	(std::cerr << ... << args) << std::endl;
}
} // namespace argon::debug
