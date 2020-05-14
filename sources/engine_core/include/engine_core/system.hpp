#pragma once

#include <memory>

#include <rttr/type.h>

#include <fundamental/helper_macros.hpp>
#include <fundamental/non_copyable.hpp>

#include "forward_declarations.hpp"

namespace argon
{
class AR_SYM_EXPORT SystemBase
	: NonCopyable
{
	AR_PRIVATE_IMPL(SystemBase);

public:
	struct ConstructionData
	{
		std::unique_ptr<SystemBasePrivate> m_impl;
	};

	SystemBase(ConstructionData &&data);
	~SystemBase();

	template <typename T>
	T& get();

private:
	friend class SystemManager; // TODO REMOVE THIS

	ServiceBase& _get(const rttr::type &type);
};

template <typename T>
T& SystemBase::get()
{
	return static_cast<T&>(_get(rttr::type::get<T>()));
}
} // namespace argon
