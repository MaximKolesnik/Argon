#pragma once

#include <engine_core/system.hpp>

#include <render/simple_app.hpp>

class SimpleAppSystem : public argon::System<SimpleAppSystem>
{
public:
	void initialize();
	void finalize();

	void tick();

private:
	argon::SimpleApp app;
};
