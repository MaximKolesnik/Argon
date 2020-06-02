cmake_minimum_required(VERSION 3.16.2)

set(BUILD_EXAMPLES OFF CACHE BOOL "Disable examples for rttr" FORCE)
set(BUILD_DOCUMENTATION OFF CACHE BOOL "Disable documentation for rttr" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "Disable unit tests for rttr" FORCE)
set(BUILD_PACKAGE OFF CACHE BOOL "Disable package for rttr" FORCE)
set(BUILD_STATIC OFF CACHE BOOL "Disable static build for rttr" FORCE)

add_subdirectory(rttr_repo)

add_library(thirdparty::rttr ALIAS rttr_core)
target_compile_options(rttr_core PRIVATE -w -fexceptions)
