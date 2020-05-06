#pragma once

#include "compiler_macros.hpp"

namespace Argon
{
#define AR_PRIVATE_IMPL(className) \
	class className##Private; \
	friend class className##Private; \
	std::unique_ptr<className##Private> m_impl

#define AR_UNUSED(expr) (void)(expr)

#define AR_PAD(bytes) \
	AR_ATTR_UNUSED byte _pad[bytes]
} // namespace Argon