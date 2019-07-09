#pragma once

#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include <fundamental/debug.hpp>
#include <fundamental/types.hpp>

#include "core_singleton.hpp"
#include "system.hpp"

namespace Argon
{
class SystemManager final
	: public CoreSingleton
{
public:
	SystemManager();
	~SystemManager();

	template <typename SystemType, typename ...Args>
	SystemType* RegisterSystem(Args &&...args);

	void tick(float32 dt);

private:
	std::unordered_map<std::type_index, std::unique_ptr<ISystemInternal>> m_systemMap;
};

template <typename SystemType, typename ...Args>
SystemType* SystemManager::RegisterSystem(Args &&...args)
{
	static_assert (std::is_class_v<SystemType>, "SystemType should be a class type");
	const std::type_index systemTypeInd(typeid(SystemType));

	if (const auto it = m_systemMap.find(systemTypeInd);
		it != m_systemMap.end())
	{
		Debug::ErrorMsg("SystemManager::RegisterSystem :: the system",
			typeid(SystemType).name(), " already exists");
		return it->second.get();
	}

	const auto inserted = m_systemMap.insert({systemTypeInd,
		std::make_unique<SystemType>(std::forward<Args>(args)...)});
	AR_ASSERT_MSG(inserted.second, "SystemManager::RegisterSystem :: failed to register");
	return inserted.first.second.get();
}
} // namespace Argon
