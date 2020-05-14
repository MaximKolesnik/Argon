#pragma once

#include <fundamental/non_copyable.hpp>

#include "forward_declarations.hpp"

namespace argon
{
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
