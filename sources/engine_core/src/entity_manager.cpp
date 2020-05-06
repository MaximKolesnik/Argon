#include "entity_manager.hpp"

namespace Argon
{
EntityManager::EntityManager() = default;

EntityManager::~EntityManager() = default;

Entity EntityManager::createEntity()
{
	return Entity(m_entitySlotGenerator.acquire());
}
} // namespace Argon
