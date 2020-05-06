#include <engine_core/reflection.hpp>

#include <fundamental/helper_macros.hpp>

#include <rttr/registration.h>

#include "plugin.hpp"

RTTR_PLUGIN_REGISTRATION
{
REGISTER_SYSTEM(SimpleAppSystem);
}

void SimpleAppSystem::initialize()
{
	app.prepare();
}

void SimpleAppSystem::finalize()
{
	app.close();
}

void SimpleAppSystem::update(Argon::float32 dt)
{
	AR_UNUSED(dt);

	app.tick();
}
