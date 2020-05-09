#pragma once

#include <type_traits>

#include <fundamental/non_copyable.hpp>

#include "ecs_traits.hpp"

namespace argon
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
		static_assert(ecstraits::HasInitializeMemberV<TDerived>, "Service must have initialize method");
		static_assert(ecstraits::HasFinalizeMemberV<TDerived>, "Service must have finalize method");
	}
};
} // namespace argon
