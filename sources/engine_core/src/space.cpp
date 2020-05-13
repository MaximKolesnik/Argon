#include <fundamental/debug.hpp>

#include "entity_manager.hpp"
#include "space.hpp"
#include "system_manager.hpp"

namespace argon
{
Space::Space(privateimpl::ServiceManager &serviceManager)
	: m_serviceManager(serviceManager)
	, m_entityManager(new EntityManager(serviceManager))
	, m_systemManager(new SystemManager(serviceManager))
{
}

Space::~Space() = default;

void Space::tick()
{
	//Debug::statusMsg("Space tick begin");
	m_systemManager->tick();
	//Debug::statusMsg("Space tick end");
}
} // namespace argon
