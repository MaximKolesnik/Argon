#pragma once

#include <memory>

#include <fundamental/non_copyable.hpp>

#include "forward_declarations.hpp"

namespace argon
{
class Space final
	: NonCopyable
{
public:
	Space(privateimpl::ServiceManager &serviceManager);
	~Space();

	void tick();

private:
	AR_ATTR_UNUSED privateimpl::ServiceManager &m_serviceManager;
	std::unique_ptr<EntityManager> m_entityManager;
	std::unique_ptr<SystemManager> m_systemManager;
};
} // namespace argon
