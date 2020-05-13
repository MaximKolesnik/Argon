#pragma once

#include <memory>

#include <rttr/type.h>

#include <fundamental/helper_macros.hpp>
#include <fundamental/non_copyable.hpp>

namespace argon
{
class AR_SYM_EXPORT ServiceBase
	: NonCopyable
{
	AR_PRIVATE_IMPL(ServiceBase);

public:
	struct ConstructionData
	{
		std::unique_ptr<ServiceBasePrivate> m_impl;
	};

	ServiceBase(ConstructionData &&data);
	~ServiceBase();

	template <typename T>
	T& get();

private:
	friend class Engine; // TODO remove this

	ServiceBase& _get(const rttr::type &type);
};

template <typename T>
T& ServiceBase::get()
{
	return static_cast<T&>(_get(rttr::type::get<T>()));
}
} // namespace argon
