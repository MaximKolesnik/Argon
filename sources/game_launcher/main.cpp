#include <engine_core/engine.hpp>
#include <engine_core/filesystem.hpp>
#include <engine_core/time.hpp>

#include <fundamental/debug.hpp>

int main(int argc, char *argv[])
{
	if (argc == 0)
	{
		argon::debug::errorMsg("Cannot extract working directory");
		return 0;
	}

	std::string basePath = argv[0];
	argon::Engine engine(basePath);

	engine.exec();
}

