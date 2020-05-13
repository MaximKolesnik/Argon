#pragma once

#include <rttr/registration.h>

#include <data_structures/standard_containers.hpp>

#include <fundamental/debug.hpp>
#include <fundamental/non_copyable.hpp>

#include "engine.hpp"

namespace argon::privateimpl
{
class ServiceData
	: NonCopyable
{
public:
	ServiceData(const rttr::type &type, rttr::variant &&object)
		: m_object(std::move(object))
		, m_tick(type.get_method("tick"))
	{
	}

	ServiceData(ServiceData &&) = default;

	~ServiceData()
	{
		m_object.get_type().destroy(m_object);
	}

	bool isValid() const
	{
		return m_tick.is_valid();
	}

	void tick()
	{
		m_tick.invoke(m_object);
	}

private:
	friend class ServiceManager;

	rttr::variant m_object;
	const rttr::method m_tick;
};

class ServiceManager final
	: NonCopyable
{
public:
	ServiceManager() = default;

	template <typename T>
	T& get()
	{
		static_assert(!std::is_pointer_v<T>, "Pointer type is not accepted");
		AR_CRITICAL(m_services.find(rttr::type::get<T>()) != m_services.end(),
			"Service cannot be located");
		
		return *m_services.at(rttr::type::get<T>()).m_object.template get_value<T*>();
	}

	ServiceBase& get(const rttr::type &type)
	{
		AR_CRITICAL(m_services.find(type) != m_services.end(),
			"Service cannot be located");
		
		return *m_services.at(type).m_object.template get_value<ServiceBase*>();
	}

private:
	friend class argon::Engine;
	friend class PluginManager;

	unordered_map<rttr::type, ServiceData> m_services;
};
} // namespace privateimpl
