#pragma once

#include <fundamental/helper_macros.hpp>
#include <fundamental/non_copyable.hpp>
#include <fundamental/types.hpp>

#include "service.hpp"

namespace argon
{
class AR_SYM_EXPORT EngineState
	: public ServiceBase
{
public:
	EngineState(ConstructionData &&data);
	~EngineState();

	void tick();

	void requestShutdown() { m_shutdown = true; }
	bool isShutingdown() const { return m_shutdown; }

private:
	bool m_shutdown;
	AR_PAD(7);
};
} // namespace argon
