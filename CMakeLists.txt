cmake_minimum_required(VERSION 3.14)

function (closure_for name)
  add_executable(parse_${name}
	cstuff/src/mmapfile.c
	src/main.c
	src/grammar.c
	src/output.c
	src/for_types.c)
  target_compile_definitions(parse_${name} PRIVATE
	-DMY_INFO="${name}_info.h")
  target_include_directories(parse_${name} PRIVATE cstuff/src)
  get_filename_component(CLOSURE_INTERFACE
	src/closure.interface.h
	ABSOLUTE
	BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  add_custom_command(
	OUTPUT ${name}.h
	COMMAND
	parse_${name} <"${CLOSURE_INTERFACE}" > ${name}.h
	MAIN_DEPENDENCY
	src/closure.interface.h
	DEPENDS
	parse_${name})
endfunction(closure_for)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  add_executable(example
	example_closure.h
	src/example_main.c)
  target_include_directories(example PRIVATE
	# XXX: forcing every _deps.h to be in src??
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	"${CMAKE_CURRENT_BINARY_DIR}"
	cstuff/src)
  closure_for(example_closure)
endif()
