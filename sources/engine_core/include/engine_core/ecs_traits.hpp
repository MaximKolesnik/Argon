#pragma once

#include <type_traits>

#include <fundamental/types.hpp>

namespace Argon::ECSTraits
{
template <typename, typename = void>
struct HasInitializeMember
{
	inline static constexpr bool value = false;
};

template <typename T>
struct HasInitializeMember<T, std::void_t<decltype(&T::initialize)>>
{
	inline static constexpr bool value =
		std::is_invocable<decltype(&T::initialize), T>::value &&
		std::is_same_v<typename std::result_of<decltype(&T::initialize)(T)>::type, void>;
};

template <typename T>
inline constexpr bool HasInitializeMemberV = HasInitializeMember<T>::value;

template <typename, typename = void>
struct HasFinalizeMember
{
	inline static constexpr bool value = false;
};

template <typename T>
struct HasFinalizeMember<T, std::void_t<decltype(&T::finalize)>>
{
	inline static constexpr bool value =
		std::is_invocable<decltype(&T::finalize), T>::value &&
		std::is_same_v<typename std::result_of<decltype(&T::finalize)(T)>::type, void>;
};

template <typename T>
inline constexpr bool HasFinalizeMemberV = HasFinalizeMember<T>::value;

template <typename, typename = void>
struct HasUpdateMember
{
	inline static constexpr bool value = false;
};

template <typename T>
struct HasUpdateMember<T, std::void_t<decltype(&T::update)>>
{
	inline static constexpr bool value =
		std::is_invocable<decltype(&T::update), T, float32>::value &&
		std::is_same_v<typename std::result_of<decltype(&T::update)(T, float32)>::type, void>;
};

template <typename T>
inline constexpr bool HasUpdateMemberV = HasUpdateMember<T>::value;
} // namespace Argon::ECSTraits
