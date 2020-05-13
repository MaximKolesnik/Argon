#include <rttr/registration.h>

#include <engine_core/filesystem.hpp>
#include <engine_core/engine_state.hpp>
#include <engine_core/reflection.hpp>

#include <fundamental/debug.hpp>
#include <fundamental/helper_macros.hpp>

#include "plugin.hpp"

RTTR_PLUGIN_REGISTRATION
{
	argon::reflection::System<SimpleAppSystem>("SimpleAppSystem");
}

void SimpleAppSystem::initialize()
{
	argon::debug::statusMsg("SimpleAppIni");
	app.prepare(
		get<argon::Filesystem>().resolveToAbsolute("/shaders/gouraud/gouraud.vert").c_str(),
		get<argon::Filesystem>().resolveToAbsolute("/shaders/gouraud/gouraud.frag").c_str());
}

void SimpleAppSystem::finalize()
{
	app.close();
	argon::debug::statusMsg("SimpleAppFini");
}

void SimpleAppSystem::tick()
{
	if (app.shouldClose())
	{
		get<argon::EngineState>().requestShutdown();
	}

	app.tick();
}
