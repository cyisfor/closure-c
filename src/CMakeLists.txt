function (closure_for name for_types_c)
  add_executable(parse_${name}
	src/grammar.c
	src/output.c
	${for_types_c})
  add_custom_command(
	OUTPUT ${name}.c
	COMMAND
	parse_${name} <closure.implementation.c > ${name}.c
	MAIN_DEPENDENCY
	closure.implementation.c
	DEPENDS
	parse_${name})
  add_custom_command(
	OUTPUT ${name}.h
	COMMAND
	parse_${name} <closure.interface.h > ${name}.h
	MAIN_DEPENDENCY
	closure.interface.h
	DEPENDS
	parse_${name})
endfunction(closure_for)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  add_executable(example,
	example_closure.c
	src/example.c)
  closure_for(example_closure src/example_for_types.c)
endif()
  
