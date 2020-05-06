#include <fundamental/debug.hpp>

#include "entity_manager.hpp"
#include "space.hpp"
#include "system_manager.hpp"

namespace Argon
{
Space::Space()
	: m_entityManager(new EntityManager)
	, m_systemManager(new SystemManager)
{
}

Space::~Space() = default;

void Space::tick()
{
	Debug::statusMsg("Space tick begin");
	m_systemManager->tick();
	Debug::statusMsg("Space tick end");
}
} // namespace Argon
