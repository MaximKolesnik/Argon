#include <rttr/registration.h>

#include "private/entity_manager_data_provider.hpp"
#include "reflection.hpp"

RTTR_REGISTRATION
{
	argon::reflection::Service<argon::privateimpl::EntityManagerDataProvider>(
		"privateimpl::EntityManagerDataProvider");
}
