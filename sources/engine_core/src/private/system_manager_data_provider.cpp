#include <rttr/registration.h>

#include "private/system_manager_data_provider.hpp"
#include "reflection.hpp"

RTTR_REGISTRATION
{
	argon::reflection::Service<argon::privateimpl::SystemManagerDataProvider>(
		"privateimpl::SystemManagerDataProvider");
}
