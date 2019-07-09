#include <fundamental/debug.hpp>

#include <render/simple_app.hpp>

#include "core.hpp"
#include "system_manager.hpp"
#include "time.hpp"

//test
namespace
{
Argon::SimpleApp simpleApp;
} // namespace


namespace Argon
{
Core *Core::s_instance = nullptr;

Core::Core()
{
	AR_CRITICAL(!s_instance, "Core can be created only once");
	s_instance = this;

	AddSingleton<Time>();
	AddSingleton<SystemManager>();

	simpleApp.prepare();
}

Core::~Core()
{
	s_instance = nullptr;

	for (auto mapEntry : m_singletonMap)
	{
		delete mapEntry.second;
	}
	m_singletonMap.clear();
}

Core& Core::GetInstance()
{
	AR_CRITICAL(s_instance, "Core is not created");
	return *s_instance;
}

void Core::tick()
{
	auto& time = Get<Time>();
	std::cout << "dt: " << time.GetDelta() << std::endl;
	std::cout << "fps: " <<  1.f / (time.GetDelta()) << std::endl;
	simpleApp.tick();
	Get<SystemManager>().tick(time.GetDelta());
	time.EndFrame();
}
} // namespace Argon
