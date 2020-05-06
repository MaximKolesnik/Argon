#pragma once

#include <engine_core/system.hpp>

#include <fundamental/types.hpp>

#include <render/simple_app.hpp>

class SimpleAppSystem : public Argon::System<SimpleAppSystem>
{
public:
	void initialize();
	void finalize();

	void update(Argon::float32 dt);

private:
	Argon::SimpleApp app;
};
