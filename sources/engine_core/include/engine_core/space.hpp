#pragma once

#include <memory>

#include <fundamental/non_copyable.hpp>

namespace argon
{
namespace privateimpl
{
class ServiceManager;
} // namespace privateimpl

class EntityManager;
class SystemManager;

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
