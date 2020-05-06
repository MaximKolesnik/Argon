#include <iostream>
#include <filesystem>

#include <fundamental/debug.hpp>

#include "private/plugin/plugin_manager.hpp"
#include "engine.hpp"
#include "filesystem.hpp"
#include "reflection.hpp"
#include "space.hpp"
#include "time.hpp"

namespace Argon
{
namespace
{
static Engine* s_engine = nullptr;
} // unnamed namespace

Engine::Engine(const std::string &workDir)
	: m_workingDir(workDir)
	, m_shutdown(false)
{
	s_engine = this;

	{
		const auto& fsType = rttr::type::get<Filesystem>();
		AR_CRITICAL(fsType.is_valid(), "Filesystem is not registered");

		m_services.emplace(fsType, fsType.create());

		auto& fs = m_services.at(fsType);
		fs.get_value<Filesystem*>()->privateConstruct(workDir);

		AR_CRITICAL(fsType.get_method("initialize").is_valid(), "Filesystem is not registered");
		fsType.get_method("initialize").invoke(fs);
	}

	m_pluginManager = std::make_unique<PluginManager>();
	m_pluginManager->initialize();

	m_space = std::make_unique<Space>();
}

Engine::~Engine()
{
	m_space.reset();
	
	for (auto &sp : m_services)
	{
		sp.first.get_method("finalize").invoke(sp.second);
		sp.first.get_destructor().invoke(sp.second);
		AR_CRITICAL(!sp.second.is_valid(), "Service was not destroyed");
	}

	m_pluginManager->finalize();

	s_engine = nullptr;
};

Engine& Engine::instance()
{
	AR_CRITICAL(s_engine, "Engine is not initialized");

	return *s_engine;
}

void Engine::exec()
{
	Time timer;

	while (!m_shutdown)
	{
		m_space->tick();
	}
}
} // namespace Argon