#include "filesystem.hpp"

#include <fundamental/debug.hpp>

#include <rttr/registration.h>

#include "reflection.hpp"

RTTR_REGISTRATION
{
	Argon::Reflection::Service<Argon::Filesystem>("Filesystem");
}

namespace Argon
{
void Filesystem::initialize()
{
	AR_CRITICAL(!m_basePath.empty(), "Base path is invalid");

	m_root = m_basePath.remove_filename()/".."/".."/"resources/";
	AR_CRITICAL(std::filesystem::exists(m_root), "Root directory cannot be resolved");
	m_root = std::filesystem::canonical(m_root);

	m_plugins = m_basePath.remove_filename()/"plugins";
	AR_CRITICAL(std::filesystem::exists(m_plugins), "Plugin directory cannot be resolved");
	m_plugins = std::filesystem::canonical(m_plugins);
}

void Filesystem::finalize()
{
}

std::filesystem::path Filesystem::resolveToAbsolute(const std::string &path) const
{
	const std::filesystem::path &abs = m_root.string() + path;
	return std::filesystem::exists(abs) ? std::filesystem::canonical(abs) : std::filesystem::path();
}

std::filesystem::path Filesystem::resolvePlugin(const std::string &path) const
{
	const std::filesystem::path &abs = m_plugins.string() + path;
	return std::filesystem::exists(abs) ? std::filesystem::canonical(abs) : std::filesystem::path();
}

void Filesystem::privateConstruct(const std::string &basePath)
{
	m_basePath = basePath;
}
} // namespace Argon
