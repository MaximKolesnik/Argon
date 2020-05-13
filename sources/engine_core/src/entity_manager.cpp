#include <data_structures/sparse_storage.hpp>

#include "private/entity_manager_data_provider.hpp"
#include "private/service_manager.hpp"
#include "entity_manager.hpp"
#include "reflection.hpp"

namespace argon
{
EntityManager::EntityManager(privateimpl::ServiceManager &serviceManager)
	: m_serviceManager(serviceManager)
	, m_impl(m_serviceManager.get<privateimpl::EntityManagerDataProvider>().acquire(this))
{
}

EntityManager::~EntityManager()
{
	m_serviceManager.get<privateimpl::EntityManagerDataProvider>().release(this);
}

Entity EntityManager::createEntity()
{
	return Entity(m_impl.m_slotGenerator.acquire());
}

bool EntityManager::isValid(const Entity& e) const
{
	return m_impl.m_slotGenerator.isValid(e.m_slot);
}
} // namespace argon
