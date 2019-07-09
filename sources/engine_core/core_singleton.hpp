#pragma once

namespace Argon
{
class CoreSingleton
{
	friend class Core;

protected:
	CoreSingleton() = default;
	virtual ~CoreSingleton() = default;
};
}
