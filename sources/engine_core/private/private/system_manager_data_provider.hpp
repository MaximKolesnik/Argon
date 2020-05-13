#pragma once

#include <rttr/registration.h>

#include <data_structures/standard_containers.hpp>

#include <fundamental/debug.hpp>
#include <fundamental/non_copyable.hpp>

#include "service.hpp"
#include "system.hpp"
#include "system_manager.hpp"

namespace argon::privateimpl
{
class SystemData final
	: NonCopyable
{
public:
	SystemData(const rttr::type &type, rttr::variant &&object)
		: m_object(std::move(object))
		, m_tick(type.get_method("tick"))
		, m_init(type.get_method("initialize"))
		, m_fini(type.get_method("finalize"))
	{
	}

	SystemData(SystemData &&) = default;

	~SystemData()
	{
		m_object.get_type().destroy(m_object);
		AR_CRITICAL(!m_object.is_valid(), "System was not destroyed");
	}

	bool isValid() const
	{
		return m_object.is_valid() && m_tick.is_valid() && m_init.is_valid()
			&& m_fini.is_valid();
	}

	void init()
	{
		m_init.invoke(m_object);
	}

	void fini()
	{
		m_fini.invoke(m_object);
	}

	void tick()
	{
		m_tick.invoke(m_object);
	}

private:
	rttr::variant m_object;
	const rttr::method m_tick;
	const rttr::method m_init;
	const rttr::method m_fini;
};

struct SystemManagerData final
	: NonCopyable
{
	unordered_map<rttr::type, SystemData> m_systems;
};

// used as an interface between system manager and hot-reloading functionality
class SystemManagerDataProvider final
	: public ServiceBase
{
public:
	SystemManagerDataProvider(ConstructionData &&data);

	SystemManagerData& acquire(SystemManager &manager);
	void release(SystemManager &manager);

	void tick() {}

private:
	unordered_map<SystemManager*, SystemManagerData> m_data;
};

inline SystemManagerDataProvider::SystemManagerDataProvider(ConstructionData &&data)
	: ServiceBase(std::move(data))
{
}

inline SystemManagerData& SystemManagerDataProvider::acquire(SystemManager &manager)
{
	AR_ASSERT_MSG(m_data.find(&manager) == m_data.end(),
		"SystemManagerDataProvider::acquire :: data is already acquired");
	return m_data[&manager];
}

inline void SystemManagerDataProvider::release(SystemManager &manager)
{
	AR_ASSERT_MSG(m_data.find(&manager) != m_data.end(),
		"SystemManagerDataProvider::acquire :: data is not acquired");
	m_data.erase(&manager);
}
} // namespace argon::privateimpl
