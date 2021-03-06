#include <rttr/library.h>

#include <fundamental/debug.hpp>

#include "engine.hpp"
#include "filesystem.hpp"
#include "private/plugin/plugin_manager.hpp"
#include "private/service_manager.hpp"

namespace argon::privateimpl
{
PluginManager::PluginManager(ServiceManager &serviceManager)
	: m_serviceManager(serviceManager)
{
}

PluginManager::~PluginManager() = default;

void PluginManager::initialize()
{
	const auto &fs = m_serviceManager.get<Filesystem>();

	const auto &pluginDir = fs.getPluginDir();

	for (const auto &p : std::filesystem::directory_iterator(pluginDir))
	{
		if (p.is_directory())
		{
			continue;
		}

		const std::string &pluginPath = p.path().generic_string();
		const std::string &filename = p.path().filename();

		auto plugin = std::make_unique<rttr::library>(pluginPath);
		if (!plugin->load())
		{
			debug::errorMsg("Failed to load plugin: ", filename, ". Error: ",
				plugin->get_error_string());
			continue;
		}

		debug::statusMsg("Plugin loaded ", filename);
		m_plugins.emplace(filename, std::move(plugin));
	}
}

void PluginManager::finalize()
{
	for (auto &[f, p] : m_plugins)
	{
		p->unload();
		p.reset();
		debug::statusMsg("Plugin unloaded ", f);
	}

	m_plugins.clear();
}
} // namespace argon::privateimpl
