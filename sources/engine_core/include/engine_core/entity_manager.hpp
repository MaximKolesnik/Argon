#pragma once

#include <data_structures/sparse_storage.hpp>

#include <fundamental/non_copyable.hpp>

#include "entity.hpp"

namespace Argon
{
class EntityManager final
	: NonCopyable
{
public:
	EntityManager();
	~EntityManager();

	Entity createEntity();


private:
	SlotGenerator m_entitySlotGenerator;
};
} // namespace Argon
