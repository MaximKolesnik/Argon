#include <data_structures/slot_map.hpp>

#include <engine_core/core.hpp>
#include <engine_core/time.hpp>

#include <fundamental/debug.hpp>

#include <render/simple_app.hpp>

struct Test2
{

};

struct Test {
	Test2 t;
	Test2 t1;
};

int main()
{
	Argon::Core engineCore;
	Argon::SlotMap<int> map;

	auto t1 = map.allocate(5);
	auto t2 = map.allocate(6);

	int &test = map.at(t1);
	int &test2 = map.at(t2);

	std::cout << test << std::endl;
	std::cout << test2 << std::endl;
	std::cout << sizeof(Test) << std::endl;
	map.erase(t1);

	while (true)
	{
		std::cout << "outside dt: " << Argon::Core::GetInstance().Get<Argon::Time>().GetDelta() << std::endl;
		std::cout << "outsidefps: " <<  1.f / (Argon::Core::GetInstance().Get<Argon::Time>().GetDelta()) << std::endl;
		engineCore.tick();
	}
}
