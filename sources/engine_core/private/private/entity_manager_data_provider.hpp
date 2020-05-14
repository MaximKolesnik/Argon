#pragma once

#include <rttr/registration>

#include <data_structures/sparse_storage.hpp>
#include <data_structures/standard_containers.hpp>

#include <fundamental/debug.hpp>

#include "forward_declarations.hpp"
#include "service.hpp"

namespace argon::privateimpl
{
struct EntityManagerData final
{
	SlotGenerator m_slotGenerator;
	unordered_map<rttr::type, rttr::variant> m_storages;
};

// Used as an interface between EntityManagers and hot-reloading functionality
class EntityManagerDataProvider final
	: ServiceBase
{
public:
	EntityManagerDataProvider(ConstructionData &&data)
		: ServiceBase(std::move(data))
	{
	}

	void tick() {}

	EntityManagerData& acquire(EntityManager *manager);
	void release(EntityManager *manager);

private:
	friend class PluginManager;

	unordered_map<EntityManager*, EntityManagerData> m_data;
};

inline EntityManagerData& EntityManagerDataProvider::acquire(EntityManager *manager)
{
	AR_ASSERT_MSG(m_data.find(manager) == m_data.end(),
		"EntityManagerDataProvider::release :: Data was already acquired");
	return m_data[manager];
}

inline void EntityManagerDataProvider::release(EntityManager *manager)
{
	AR_ASSERT_MSG(m_data.find(manager) != m_data.end(),
		"EntityManagerDataProvider::release :: Data was not acquired");
	m_data.erase(manager);
}
} // namespace argon::privateimpl
