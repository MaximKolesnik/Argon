#pragma once

#include <type_traits>

#include <fundamental/non_copyable.hpp>

#include "ecs_traits.hpp"

namespace Argon
{
template <typename TDerived>
struct Service : NonCopyable
{
	Service()
	{
		static_assert(std::is_default_constructible_v<TDerived>,
			"Service must be default constructible");
		static_assert(!std::is_polymorphic_v<TDerived>);
		static_assert(!std::is_copy_constructible_v<TDerived>
			&& !std::is_assignable_v<TDerived, TDerived>
			&& !std::is_copy_assignable_v<TDerived>, "Service cannot be copyable");
		static_assert(ECSTraits::HasInitializeMemberV<TDerived>, "Service must have initialize method");
		static_assert(ECSTraits::HasFinalizeMemberV<TDerived>, "Service must have finalize method");
	}
};
} // namespace Argon
