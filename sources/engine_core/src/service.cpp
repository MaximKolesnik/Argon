#include "private/construction_data_impl.hpp"
#include "service.hpp"

namespace argon
{
ServiceBase::ServiceBase(ConstructionData &&data)
	: m_impl(std::move(data.m_impl))
{
}

ServiceBase::~ServiceBase() = default;

ServiceBase& ServiceBase::_get(const rttr::type &type)
{
	return m_impl->m_serviceManager.get(type);
}
} // namespace argon
