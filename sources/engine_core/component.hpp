#pragma once

#include <typeindex>

#include <fundamental/compiler_macros.hpp>
#include <fundamental/non_copyable.hpp>
#include <fundamental/types.hpp>

namespace Argon
{
template <typename CompType>
class Handle;

class IComponentInternal
	: public Argon::NonCopyable
{
public:
	using CompType = uint64;

protected:
	IComponentInternal();
	~IComponentInternal();
	static CompType s_typeCounter;
	static CompType GetType(const std::type_index& typeInd);
};

template <typename DerivedT>
class IComponent
	: public IComponentInternal
{
	using Base = IComponentInternal;

public:
	static IComponentInternal::CompType GetType()
	{
		static auto type = Base::GetType(std::type_index(typeid(DerivedT)));
		return type;
	}

private:
};
} // namespace Argon
