#pragma once

#include <memory>

#include <fundamental/non_copyable.hpp>

namespace argon
{
class EntityManager;
class SystemManager;

class Space final
	: NonCopyable
{
public:
	Space();
	~Space();

	void tick();

private:
	std::unique_ptr<EntityManager> m_entityManager;
	std::unique_ptr<SystemManager> m_systemManager;
};
} // namespace argon
