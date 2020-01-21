#pragma once

#include <typeindex>

#include <fundamental/non_copyable.hpp>
#include <fundamental/types.hpp>

namespace Argon
{
class ISystemInternal
	: public NonCopyable
{
public:
	using SystemType = uint64;

	~ISystemInternal();

protected:
	static SystemType s_typeCounter;
	static SystemType getType(const std::type_index &typeInd);
};

template <typename DerivedT>
class ISystem
	: public ISystemInternal
{
	using Base = ISystemInternal;

public:
	virtual ~ISystem() = default;

	static ISystemInternal::SystemType getType()
	{
		static auto type = Base::getType(std::type_index(typeid(DerivedT)));
		return type;
	}

	virtual void update(float32 dt) = 0;

private:
};
} // namespace Argon
