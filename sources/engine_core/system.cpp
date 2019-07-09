#include <unordered_map>

#include <fundamental/debug.hpp>

#include "system.hpp"

namespace Argon
{
ISystemInternal::~ISystemInternal() = default;

ISystemInternal::SystemType ISystemInternal::s_typeCounter = 0;

ISystemInternal::SystemType ISystemInternal::GetType(const std::type_index &typeInd)
{
	static std::unordered_map<std::type_index, ISystemInternal::SystemType> m_typeMap = {};

	const auto it = m_typeMap.find(typeInd);
	if (it == m_typeMap.end())
	{
		const auto inserted = m_typeMap.insert({typeInd, s_typeCounter++});
		AR_ASSERT_MSG(inserted.second,
			"ISystemInternal::GetType :: cannot register system");
		return inserted.first->second;
	}

	return it->second;
}
} // namespace Argon
