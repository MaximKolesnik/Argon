#include "entity.hpp"

namespace argon
{
Entity::Entity()
	: m_slot()
{
}

Entity::Entity(SlotGenerator::Slot slot)
	: m_slot(std::move(slot))
{
}
} // namespace argon
