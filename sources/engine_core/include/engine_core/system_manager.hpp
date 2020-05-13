#pragma once

#include <fundamental/non_copyable.hpp>

namespace argon
{
namespace privateimpl
{
struct SystemManagerData;
class ServiceManager;
} // namespace privateimpl

class SystemManager
	: NonCopyable
{
public:
	SystemManager(privateimpl::ServiceManager &serviceManager);
	~SystemManager();

	void tick();

private:
	privateimpl::ServiceManager &m_serviceManager;
	privateimpl::SystemManagerData &m_data;
};
} // namespace argon
