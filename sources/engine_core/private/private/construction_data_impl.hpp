#include "service.hpp"
#include "system.hpp"
#include "private/service_manager.hpp"

namespace argon
{
class SystemBase::SystemBasePrivate final
{
public:
	SystemBasePrivate(privateimpl::ServiceManager &serviceManager)
		: m_serviceManager(serviceManager)
	{
	}

	privateimpl::ServiceManager &m_serviceManager;
};

class ServiceBase::ServiceBasePrivate final
{
public:
	ServiceBasePrivate(privateimpl::ServiceManager &serviceManager)
		: m_serviceManager(serviceManager)
	{
	}

	privateimpl::ServiceManager &m_serviceManager;
};
} // namespace argon
