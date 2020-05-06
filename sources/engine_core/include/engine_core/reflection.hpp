#pragma once

#include <type_traits>

namespace Argon::Reflection
{
inline constexpr const char* C_META_CLASS_TYPE = "ClassType";

enum class ClassType
{
	Service,
	System
};
} // namespace Argon::Reflection

#define REGISTER_SERVICE(Type) \
static_assert(std::is_base_of_v<Argon::Service<Type>, Type>, \
	"Service must be derived from service template"); \
rttr::registration::class_<Type>(#Type) \
.constructor<>() \
(rttr::policy::ctor::as_raw_ptr) \
(rttr::metadata(Argon::Reflection::C_META_CLASS_TYPE, Argon::Reflection::ClassType::Service)) \
.method("initialize", &Type::initialize) \
.method("finalize", &Type::finalize)

// TODO consider removing initialize and finalize from the System

#define REGISTER_SYSTEM(Type) \
static_assert(std::is_base_of_v<Argon::System<Type>, Type>, \
	"System must be derived from system template"); \
rttr::registration::class_<Type>(#Type) \
.constructor<>() \
(rttr::policy::ctor::as_raw_ptr) \
(rttr::metadata(Argon::Reflection::C_META_CLASS_TYPE, Argon::Reflection::ClassType::System)) \
.method("initialize", &Type::initialize) \
.method("finalize", &Type::finalize) \
.method("tick", &Type::tick)
