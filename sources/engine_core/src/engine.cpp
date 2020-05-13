#include <fundamental/debug.hpp>

#include "private/construction_data_impl.hpp"
#include "private/plugin/plugin_manager.hpp"
#include "private/service_manager.hpp"
#include "engine.hpp"
#include "engine_state.hpp"
#include "filesystem.hpp"
#include "reflection.hpp"
#include "service.hpp"
#include "space.hpp"
#include "time.hpp"

namespace argon
{
Engine::Engine(const std::string &workDir)
	: m_workingDir(workDir)
	, m_serviceManager(new privateimpl::ServiceManager)
	, m_pluginManager(new privateimpl::PluginManager(*m_serviceManager))
{
	_initCoreServices();
	m_pluginManager->initialize();
	m_space = std::make_unique<Space>(*m_serviceManager);
}

Engine::~Engine()
{
	m_space.reset();
	m_pluginManager->finalize();
	m_serviceManager.reset();
}

void Engine::exec()
{
	Time timer;

	while (!m_serviceManager->get<EngineState>().isShutingdown())
	{
		m_space->tick();
	}
}

void Engine::_initCoreServices()
{
	for (const auto &type : rttr::type::get_types())
	{
		const rttr::variant classTypeMeta = type.get_metadata(reflection::ServiceMeta::Type);
		if (!classTypeMeta.is_valid())
		{
			continue;
		}

		const auto classType = classTypeMeta.get_value<reflection::ClassType>();
		if (classType == reflection::ClassType::Service)
		{
			const auto ctr = type.get_constructor({rttr::type::get<ServiceBase::ConstructionData&&>()});
			auto var = ctr.invoke(ServiceBase::ConstructionData{
				std::make_unique<ServiceBase::ServiceBasePrivate>(*m_serviceManager)});

			privateimpl::ServiceData data(type, std::move(var));
			m_serviceManager->m_services.emplace(type, std::move(data));
		}
	}

	m_serviceManager->get<Filesystem>()._privateConstruct(m_workingDir);
}
} // namespace argon
