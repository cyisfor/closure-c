cmake_minimum_required(VERSION 3.14)

add_subdirectory(data_to_header)

data_to_header(TEMPLATE
  TARGET TEMPLATE
  HEADER template.h
  SOURCE src/closure.template.h)

function (parse_for name info)
  add_executable(parse_${name}
	cstuff/src/mmapfile.c
	src/main.c
	src/grammar.c
	src/output.c
	src/for_types.c)
  add_dependencies(parse_${name} TEMPLATE)
  get_filename_component(info "${info}" ABSOLUTE BASE_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}")
  target_compile_definitions(parse_${name} PRIVATE
	-DMY_INFO="${info}")
  target_include_directories(parse_${name} PRIVATE
	"${CMAKE_CURRENT_BINARY_DIR}"
	cstuff/src)
endfunction(parse_for)

function (closure_for name)
  cmake_parse_arguments(PARSE_ARGV 1 A "" "INFO" "")
  if(NOT A_INFO)
	set(A_INFO "src/${name}_info.h")
  endif()
  parse_for(${name} ${A_INFO})
  add_custom_command(
	OUTPUT ${name}.h
	COMMAND
	parse_${name} > ${name}.h
	MAIN_DEPENDENCY
	"${A_INFO}"
	DEPENDS
	parse_${name})
  add_custom_target(
	"${name}_closure"
	DEPENDS
	"${name}.h")
endfunction(closure_for)

parse_for(script src/script_info.h)
target_sources(parse_script PRIVATE
  src/script_info.c)

function (closure_script name)
  cmake_parse_arguments(PARSE_ARGV 1 A "" "SCRIPT" "")
  if(NOT A_SCRIPT)
	set(A_SCRIPT "${name}")
  endif()
  get_filename_component(A_SCRIPT "${A_SCRIPT}" ABSOLUTE BASE_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}")
  add_custom_command(
	OUTPUT "${name}.h"
	COMMAND
	parse_script < "${A_SCRIPT}" > "${name}.h")
  add_custom_target(
	"${name}_closure"
	DEPENDS
	"${name}.h")
endfunction()

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  include(FindPkgConfig)
  pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)
  closure_for(example_closure
	INFO src/example/closure_info.h)
  add_executable(example
	src/example/main.c)
  add_dependencies(example example_closure_closure)
  target_include_directories(example PRIVATE
	# XXX: forcing every _deps.h to be in src??
	"${CMAKE_CURRENT_SOURCE_DIR}/src/example"
	"${CMAKE_CURRENT_BINARY_DIR}"
	cstuff/src)
  target_link_libraries(example PkgConfig::GLIB)
  
  closure_script(example2 SCRIPT src/example2.script)
  add_executable(example2
	src/example2.c
	example2.h)
  add_dependencies(example2 example2_closure)
endif()
