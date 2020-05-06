#pragma once

#include <data_structures/sparse_storage.hpp>

namespace Argon
{
class Entity final
{
public:
	Entity();

	uint32 getIndex() const { return m_slot.getIndex(); }
	uint64 getGeneration() const { return m_slot.getGeneration(); }

private:
	friend class EntityManager;
	
	Entity(SlotGenerator::Slot slot);

	SlotGenerator::Slot m_slot;
};
} // namespace Argon
