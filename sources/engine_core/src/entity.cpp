#include "entity.hpp"

namespace Argon
{
Entity::Entity()
	: m_slot()
{
}

Entity::Entity(SlotGenerator::Slot slot)
	: m_slot(std::move(slot))
{
}
} // namespace Argon
