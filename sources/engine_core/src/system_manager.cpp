#include <fundamental/debug.hpp>

#include "system_manager.hpp"
#include "system.hpp"

namespace argon
{
SystemManager::SystemManager()
{
	const auto sysType = rttr::type::get_by_name("SimpleAppSystem");
	AR_CRITICAL(sysType.is_valid(), "Filesystem is not registered");

	m_systems.emplace(sysType, sysType.create());
	sysType.get_method("initialize").invoke(m_systems[sysType]);
}

SystemManager::~SystemManager()
{
	for (auto &s : m_systems)
	{
		s.first.get_method("finalize").invoke(s.second);
		s.first.get_destructor().invoke(s.second);
		AR_CRITICAL(!s.second.is_valid(), "System was not destroyed");
	}

	m_systems.clear();
}

void SystemManager::tick()
{
	for (const auto& s : m_systems)
	{
		s.first.get_method("tick").invoke(s.second);
	}
}
} // namespace argon
