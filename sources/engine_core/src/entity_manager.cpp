#include <data_structures/sparse_storage.hpp>

#include "entity_manager.hpp"
#include "reflection.hpp"

namespace Argon
{
EntityManager::EntityManager()
	: m_entitySlotGenerator(new SlotGenerator)
{
}

EntityManager::~EntityManager() = default;

Entity EntityManager::createEntity()
{
	return Entity(m_entitySlotGenerator->acquire());
}

bool EntityManager::isValid(const Entity& e) const
{
	return m_entitySlotGenerator->isValid(e.m_slot);
}
} // namespace Argon
