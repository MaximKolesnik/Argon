#pragma once

#include <memory>

#include <fundamental/non_copyable.hpp>

#include "entity.hpp"

namespace argon
{
namespace privateimpl
{
struct EntityManagerData;
class ServiceManager;
} // namespace privateimpl

class EntityManager final
	: NonCopyable
{
public:
	EntityManager(privateimpl::ServiceManager &serviceManager);
	~EntityManager();

	Entity createEntity();
	bool isValid(const Entity& e) const;

private:
	privateimpl::ServiceManager &m_serviceManager;
	privateimpl::EntityManagerData& m_impl;
};
} // namespace argon
