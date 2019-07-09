#pragma once

#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include <fundamental/debug.hpp>
#include <fundamental/compiler_macros.hpp>
#include <fundamental/non_copyable.hpp>

#include "forward_declarations.hpp"

namespace Argon
{
class AR_SYM_EXPORT Core final
	: public NonCopyable
{
public:
	Core();
	~Core();

	static Core& GetInstance();

	template <typename SType, typename ...Args>
	void AddSingleton(Args &&...args);
	template <typename SType>
	SType& Get();

	void tick();

private:
	static Core *s_instance;

	std::unordered_map<std::type_index, CoreSingleton*> m_singletonMap;
};

template <typename SType, typename ...Args>
void Core::AddSingleton(Args &&...args)
{
	static_assert(std::is_base_of_v<CoreSingleton, SType>,
		"Singleton must be derived from CoreSingleton");
	const std::type_index singletonTypeInd(typeid(SType));

	if (const auto it = m_singletonMap.find(singletonTypeInd);
		it != m_singletonMap.end())
	{
		Debug::ErrorMsg("Singleton ", typeid(SType).name(), " is already registered");
		return;
	}

	const auto inserted =m_singletonMap.insert({singletonTypeInd,
		new SType(std::forward<Args>(args)...)});
	AR_ASSERT_MSG(inserted.second, "Core::AddSingleton :: failed to register");
}

template <typename SType>
SType& Core::Get()
{
	static_assert(std::is_base_of_v<CoreSingleton, SType>,
		"Singleton must be derived from CoreSingleton");

	const auto it = m_singletonMap.find(std::type_index(typeid(SType)));
	AR_CRITICAL(it != m_singletonMap.end(), "Core::Get :: Singleton is not registered");

	return *static_cast<SType*>(it->second);
}
} // namespace Argon
