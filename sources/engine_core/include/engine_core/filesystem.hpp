#pragma once

#include <filesystem>
#include <string>

#include <fundamental/compiler_macros.hpp>

#include "service.hpp"

namespace argon
{
class AR_SYM_EXPORT Filesystem final
	: public ServiceBase
{
public:
	Filesystem(ConstructionData &&data);
	~Filesystem();
	
	void tick();

	const std::filesystem::path& getPluginDir() const { return m_plugins; }

	std::filesystem::path resolveToAbsolute(const std::string &path) const;
	std::filesystem::path resolvePlugin(const std::string &path) const;

private:
	friend class Engine;

	void _privateConstruct(const std::string &basePath);

	std::filesystem::path m_root;
	std::filesystem::path m_plugins;
	std::filesystem::path m_basePath;
};
} // namespace argon
