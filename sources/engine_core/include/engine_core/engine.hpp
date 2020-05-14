#pragma once

#include <memory>
#include <string>

#include <rttr/registration.h>

#include <data_structures/standard_containers.hpp>

#include <fundamental/compiler_macros.hpp>
#include <fundamental/non_copyable.hpp>
#include <fundamental/types.hpp>

#include "forward_declarations.hpp"
#include "service.hpp"

namespace argon
{
class AR_SYM_EXPORT Engine final
	: NonCopyable
{
public:
	Engine(const std::string &workDir);
	~Engine();

	void exec();

private:
	void _initCoreServices();

	// TODO move to impl
	const std::string m_workingDir;
	std::unique_ptr<privateimpl::ServiceManager> m_serviceManager;
	std::unique_ptr<privateimpl::PluginManager> m_pluginManager;
	std::unique_ptr<Space> m_space;
};
} // namespace argon
