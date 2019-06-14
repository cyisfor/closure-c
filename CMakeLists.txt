cmake_minimum_required(VERSION 3.14)
project(closure VERSION 0.1)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  add_subdirectory(cstuff)
  add_cstuff(mystring)
  include(FindPkgConfig)
  pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)
  add_executable(example
	example/main.c)
  target_link_libraries(example PRIVATE cstuff PkgConfig::GLIB)
  target_compile_options(example PRIVATE -fplan9-extensions)
endif()
