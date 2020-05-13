#include "engine_state.hpp"
#include "reflection.hpp"

RTTR_REGISTRATION
{
argon::reflection::Service<argon::EngineState>("EngineState");
}

namespace argon
{
EngineState::EngineState(ConstructionData &&data)
	: ServiceBase(std::move(data))
	, m_shutdown(false)
{
}

EngineState::~EngineState() = default;

void EngineState::tick()
{
}
} // namespace argon
