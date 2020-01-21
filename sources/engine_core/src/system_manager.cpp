#include "system_manager.hpp"

namespace Argon
{
SystemManager::SystemManager() = default;

SystemManager::~SystemManager() = default;

void SystemManager::tick(float32 dt)
{
	(void)dt;
	//Debug::StatusMsg("System Manager Update ", dt);
}
} // namespace Argon
