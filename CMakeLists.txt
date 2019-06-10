cmake_minimum_required(VERSION 3.14)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  include(FindPkgConfig)
  pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)
  add_executable(example
	src/example/main.c)
  target_link_libraries(example PkgConfig::GLIB)
  target_include_directories(example PRIVATE cstuff/src)
  target_compile_options(example PUBLIC -fplan9-extensions)
				  
endif()
