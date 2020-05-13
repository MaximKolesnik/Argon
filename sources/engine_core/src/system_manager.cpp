#include <fundamental/debug.hpp>

#include "private/system_manager_data_provider.hpp"
#include "private/construction_data_impl.hpp"
#include "system_manager.hpp"
#include "system.hpp"

namespace argon
{
SystemManager::SystemManager(privateimpl::ServiceManager &serviceManager)
	: m_serviceManager(serviceManager)
	, m_data(serviceManager.get<privateimpl::SystemManagerDataProvider>().acquire(*this))
{
	const auto sysType = rttr::type::get_by_name("SimpleAppSystem");
	AR_CRITICAL(sysType.is_valid(), "Filesystem is not registered");

	const auto ctr = sysType.get_constructor({rttr::type::get<SystemBase::ConstructionData&&>()});
	auto var = ctr.invoke(SystemBase::ConstructionData{
		std::make_unique<SystemBase::SystemBasePrivate>(m_serviceManager)});

	privateimpl::SystemData data(sysType, std::move(var));
	AR_CRITICAL(data.isValid(), "SystemData is not valid");

	data.init();
	m_data.m_systems.emplace(sysType, std::move(data));
}

SystemManager::~SystemManager()
{
	for (auto &s : m_data.m_systems)
	{
		s.second.fini();
	}

	m_data.m_systems.clear();
	m_serviceManager.get<privateimpl::SystemManagerDataProvider>().release(*this);
}

void SystemManager::tick()
{
	for (auto& s : m_data.m_systems)
	{
		s.second.tick();
	}
}
} // namespace argon
