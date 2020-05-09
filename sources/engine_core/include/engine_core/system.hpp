#pragma once

#include <typeindex>

#include <fundamental/non_copyable.hpp>
#include <fundamental/types.hpp>

#include "ecs_traits.hpp"

// TODO consider removing initialize and finalize from the System

namespace argon
{
template <typename TDerived>
struct System : NonCopyable
{
	System()
	{
		static_assert(!std::is_polymorphic_v<TDerived>);
		static_assert(!std::is_copy_constructible_v<TDerived>
			&& !std::is_assignable_v<TDerived, TDerived>
			&& !std::is_copy_assignable_v<TDerived>, "System cannot be copyable");
		static_assert(ecstraits::HasTickMemberV<TDerived>, "System must have tick method");
		static_assert(ecstraits::HasInitializeMemberV<TDerived>, "Service must have initialize method");
		static_assert(ecstraits::HasFinalizeMemberV<TDerived>, "Service must have finalize method");
	}
};
} // namespace argon
