#pragma once

#include <engine_core/system.hpp>

#include <render/simple_app.hpp>

#include <fundamental/helper_macros.hpp>
#include <fundamental/types.hpp>

class SimpleAppSystem : public argon::SystemBase
{
public:
	SimpleAppSystem(argon::SystemBase::ConstructionData &&data)
		: argon::SystemBase(std::move(data)) {}

	void initialize();
	void finalize();

	void tick();

private:
	argon::SimpleApp app;
	AR_PAD(7);
};
