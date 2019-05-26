#include <engine_core/component.hpp>
#include <engine_core/time.hpp>
#include <fundamental/debug.hpp>

#include <render/simple_app.hpp>

int main()
{
	Argon::Core::Time time;
	Argon::SimpleApp simpleApp;
	simpleApp.prepare();

	while (!simpleApp.shouldClose())
	{
		std::cout << "dt: " << time.getDelta() << std::endl;
		std::cout << "fps: " <<  1.f / (time.getDelta()) << std::endl;
		simpleApp.tick();
		time.endFrame();
	}

	simpleApp.close();
}
