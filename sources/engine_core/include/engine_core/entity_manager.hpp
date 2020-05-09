#pragma once

#include <memory>

#include <fundamental/non_copyable.hpp>

#include "entity.hpp"

namespace argon
{
class SlotGenerator;

class EntityManager final
	: NonCopyable
{
public:
	EntityManager();
	~EntityManager();

	Entity createEntity();
	bool isValid(const Entity& e) const;

private:
	std::unique_ptr<SlotGenerator> m_entitySlotGenerator;
};
} // namespace argon
