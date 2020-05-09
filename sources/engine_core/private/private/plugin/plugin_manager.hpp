#pragma once

#include <memory>
#include <string>

#include <data_structures/standard_containers.hpp>

#include <fundamental/non_copyable.hpp>

namespace rttr
{
class library;
} // namespace rttr

namespace argon
{
class PluginManager
	: NonCopyable
{
public:
	PluginManager();
	~PluginManager();

	void initialize();
	void finalize();

private:
	unordered_map<std::string, std::unique_ptr<rttr::library>> m_plugins;
};
} // namespace argon
