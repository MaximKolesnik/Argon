#include <unordered_map>

#include <fundamental/debug.hpp>

#include "component.hpp"

namespace Argon
{
IComponentInternal::IComponentInternal() = default;

IComponentInternal::~IComponentInternal() = default;

IComponentInternal::CompType IComponentInternal::s_typeCounter = 0;

IComponentInternal::CompType IComponentInternal::getType(const std::type_index& typeInd)
{
	static std::unordered_map<std::type_index, IComponentInternal::CompType> m_typeMap = {};

	const auto it = m_typeMap.find(typeInd);
	if (it == m_typeMap.end())
	{
		const auto inserted = m_typeMap.insert({typeInd, s_typeCounter++});
		AR_ASSERT_MSG(inserted.second,
			"IComponentInternal::GetType :: cannot register component");
		return inserted.first->second;
	}

	return it->second;
}
} // namespace Argon
