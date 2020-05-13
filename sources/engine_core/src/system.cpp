#include "system.hpp"
#include "private/construction_data_impl.hpp"

#include <rttr/registration.h>

namespace argon
{
// SystemBase::ConstructionData::ConstructionData() = default;

// SystemBase::ConstructionData::~ConstructionData() = default;

SystemBase::SystemBase(ConstructionData &&data)
	: m_impl(std::move(data.m_impl))
{
}

SystemBase::~SystemBase() = default;

ServiceBase& SystemBase::_get(const rttr::type &type)
{
	return m_impl->m_serviceManager.get(type);
}
} // namespace argon
