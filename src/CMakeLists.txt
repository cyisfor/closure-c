function (closure_for name)
  add_executable(parse_${name}
	cstuff/src/mmapfile.c
	src/main.c
	src/grammar.c
	src/output.c
	src/for_types.c)
  target_compile_definitions(parse_${name}
	-DMY_INFO="${name}_info.h")
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
	example_closure.h
	src/example_main.c)
  closure_for(example_closure)
endif()
