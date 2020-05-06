#pragma once

#include <engine_core/system.hpp>

#include <render/simple_app.hpp>

class SimpleAppSystem : public Argon::System<SimpleAppSystem>
{
public:
	void initialize();
	void finalize();

	void tick();

private:
	Argon::SimpleApp app;
};
