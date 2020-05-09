#include <rttr/registration.h>

#include <engine_core/reflection.hpp>

#include <fundamental/helper_macros.hpp>


#include "plugin.hpp"

RTTR_PLUGIN_REGISTRATION
{
	Argon::Reflection::System<SimpleAppSystem>("SimpleAppSystem");
}

void SimpleAppSystem::initialize()
{
	app.prepare();
}

void SimpleAppSystem::finalize()
{
	app.close();
}

void SimpleAppSystem::tick()
{
	app.tick();
}
