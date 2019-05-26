#pragma once

#include <typeindex>

#include <fundamental/compiler_macros.hpp>
#include <fundamental/non_copyable.hpp>
#include <fundamental/types.hpp>

namespace Argon::Core
{
template <typename CompType>
class Handle;

class AR_SYM_EXPORT IComponentInternal
	: public Argon::NonCopyable
{
public:
	using CompType = uint64;

protected:
	IComponentInternal();
	~IComponentInternal();
	static CompType s_typeCounter;
	static CompType getType(const std::type_index& typeInd);
};

template <typename DerivedT>
class IComponent
	: public IComponentInternal
{
	using Base = IComponentInternal;

public:
	static IComponentInternal::CompType getType()
	{
		static auto type = Base::getType(std::type_index(typeid(DerivedT)));
		return type;
	}

private:
};
} // namespace Argon::Core
