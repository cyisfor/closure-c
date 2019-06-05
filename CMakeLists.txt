cmake_minimum_required(VERSION 3.14)

add_subdirectory(data_to_header)

data_to_header(TEMPLATE
  TARGET TEMPLATE
  HEADER template.h
  SOURCE src/closure.interface.h)

function (parse_for name)
  add_executable(parse_${name}
	cstuff/src/mmapfile.c
	src/main.c
	src/grammar.c
	src/output.c
	src/for_types.c)
  add_dependencies(parse_${name} TEMPLATE)
  target_compile_definitions(parse_${name} PRIVATE
	-DMY_INFO="${name}_info.h")
  target_include_directories(parse_${name} PRIVATE
	"${CMAKE_CURRENT_BINARY_DIR}"
	cstuff/src)
endfunction(parse_for)

function (closure_for name)
  parse_for(${name})
  add_custom_command(
	OUTPUT ${name}.h
	COMMAND
	parse_${name} > ${name}.h
	DEPENDS
	parse_${name})
  
endfunction(closure_for)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  include(FindPkgConfig)
  pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)
  add_executable(example
	example_closure.h
	src/example_main.c)
  target_include_directories(example PRIVATE
	# XXX: forcing every _deps.h to be in src??
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	"${CMAKE_CURRENT_BINARY_DIR}"
	cstuff/src)
  target_link_libraries(example PkgConfig::GLIB)
  closure_for(example_closure)
endif()

parse_for(script)
