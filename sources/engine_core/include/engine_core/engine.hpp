#pragma once

#include <memory>
#include <string>

#include <rttr/registration.h>

#include <data_structures/standard_containers.hpp>

#include <fundamental/compiler_macros.hpp>
#include <fundamental/non_copyable.hpp>

#include "service.hpp"

// TODO remove Engine singleton asap!

namespace Argon
{
class PluginManager;

class AR_SYM_EXPORT Engine final
	: NonCopyable
{
public:
	Engine(const std::string &workDir);
	~Engine();

	static Engine& instance();

	void exec();

	// TODO after removing singleton move services to some sort of accessor
	template <typename TService>
	TService& get() const;

private:
	const std::string m_workingDir;
	unordered_map<rttr::type, rttr::variant> m_services;
	std::unique_ptr<PluginManager> m_pluginManager;
};

template <typename TService>
TService& Engine::get() const
{
	static_assert(std::is_base_of_v<Service<TService>, TService>,
		"Service must be derived from service template");
	return *m_services.at(rttr::type::get<TService>()).template get_value<TService*>();
}
} // namespace Argon
