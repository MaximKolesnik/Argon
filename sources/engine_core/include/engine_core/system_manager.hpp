#pragma once

#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include <fundamental/debug.hpp>
#pragma once

#include <memory>

#include <rttr/registration.h>

#include <data_structures/standard_containers.hpp>

#include <fundamental/non_copyable.hpp>

namespace Argon
{
class SystemManager
	: NonCopyable
{
public:
	SystemManager();
	~SystemManager();

	void tick();

private:
	unordered_map<rttr::type, rttr::variant> m_systems;
};
} // namespace Argon
