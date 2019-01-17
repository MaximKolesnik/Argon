#pragma once

#include <cassert>

//TODO fix AR_ASSERT_MSG

#define AR_ASSERT(expr) assert(expr)
#define AR_ASSERT_MSG(expr, msg) assert(expr)
