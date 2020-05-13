#pragma once

#include <memory>
#include <string>

#include <data_structures/standard_containers.hpp>

#include <fundamental/non_copyable.hpp>

namespace rttr
{
class library;
} // namespace rttr

namespace argon::privateimpl
{
class ServiceManager;

class PluginManager
	: NonCopyable
{
public:
	PluginManager(ServiceManager &serviceManager);
	~PluginManager();

	void initialize();
	void finalize();

private:
	ServiceManager &m_serviceManager;
	unordered_map<std::string, std::unique_ptr<rttr::library>> m_plugins;
};
} // namespace argon::privateimpl
