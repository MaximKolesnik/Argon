#pragma once

#include <memory>
#include <limits>
#include <type_traits>

#include <rttr/registration.h>

#include <fundamental/compiler_macros.hpp>
#include <fundamental/debug.hpp>
#include <fundamental/helper_macros.hpp>
#include <fundamental/types.hpp>

namespace argon
{
template <typename> struct System;
template <typename> struct Service;
} // namespace argon

namespace argon::reflection
{
namespace detail
{
static constexpr uint64 C_META_REGISTERED = std::numeric_limits<uint64>::max();

template <typename T>
class Object
{
public:
	Object(const char *name)
	{
		static_assert(!std::is_pointer_v<T>, "Pointers cannot be registered");

		AR_CRITICAL(!rttr::type::get_by_name(name).is_valid(), "Name is already registered");
		AR_CRITICAL(rttr::type::get<T>().is_valid()
			&& !rttr::type::get<T>().get_metadata(C_META_REGISTERED).is_valid(), "Type is already registered");

		m_class = std::make_unique<rttr::registration::class_<T>>(name);
		(*m_class)(rttr::metadata(C_META_REGISTERED, {}));
	}

protected:
	std::unique_ptr<rttr::registration::class_<T>> m_class;
};
} // namespace detail

inline constexpr const char* C_META_CLASS_TYPE = "ClassType";

enum class ClassType : uint32
{
	Service = 0,
	System,
	Component
};

enum class ComponentMeta : uint32
{
	Type = 0
};

enum class ServiceMeta : uint32
{
	Type = 0
};

enum class SystemMeta : uint32
{
	Type = 0
};

template <typename T>
class Component final
	: detail::Object<T>
{
public:
	Component(const char *name);
};

template <typename T>
class Service final
	: detail::Object<T>
{
public:
	Service(const char *name);
};

template <typename T>
class System final
	: detail::Object<T>
{
public:
	System(const char *name);
};

template <typename T>
Component<T>::Component(const char *name)
	: detail::Object<T>(name)
{
	static_assert(std::is_copy_assignable_v<T> && std::is_copy_constructible_v<T>,
			"Component must be copyable");

	debug::statusMsg("Component ", name, " registered");
	this->m_class->template constructor<>()
		(rttr::policy::ctor::as_raw_ptr)
		(rttr::metadata(ComponentMeta::Type, ClassType::Component));
}

template <typename T>
Service<T>::Service(const char *name)
	: detail::Object<T>(name)
{
	static_assert(std::is_base_of_v<argon::Service<T>, T>,
		"Service must be derived from service template");

	debug::statusMsg("Service ", name, " registered");
	this->m_class->template constructor<>()
		(rttr::policy::ctor::as_raw_ptr)
		(rttr::metadata(ServiceMeta::Type, ClassType::Service))
		.method("initialize", &T::initialize)
		.method("finalize", &T::finalize);
}

template <typename T>
System<T>::System(const char *name)
	: detail::Object<T>(name)
{
	static_assert(std::is_base_of_v<argon::System<T>, T>,
		"System must be derived from system template");

	debug::statusMsg("System ", name, " registered");
	this->m_class->template constructor<>()
		(rttr::policy::ctor::as_raw_ptr)
		(rttr::metadata(SystemMeta::Type, ClassType::System))
		.method("initialize", &T::initialize)
		.method("finalize", &T::finalize)
		.method("tick", &T::tick);
}

} // namespace argon::reflection
