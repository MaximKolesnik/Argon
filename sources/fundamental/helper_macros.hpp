#pragma once

namespace Argon
{
namespace Windowing
{
#define AR_PRIVATE_IMPL(className) \
	class className##Private; \
	friend class className##Private; \
	std::unique_ptr<className##Private> m_impl

#define AR_UNUSED(expr) (void)(expr)
} // namespace Windowing
} // namespace Argon
