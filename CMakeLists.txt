cmake_minimum_required(VERSION 3.14)

add_subdirectory(data_to_header)

data_to_header(TEMPLATE
  TARGET TEMPLATE
  HEADER template.h
  SOURCE src/closure.template.h)

add_library(libparse
  src/showbuf.c # debugging
  	cstuff/src/mmapfile.c
	src/parse_for_types.c
	src/parse_for_expression.c
	src/parse.c)
target_compile_options(libparse PUBLIC -fplan9-extensions)
add_dependencies(libparse TEMPLATE)
target_include_directories(libparse PUBLIC
	"${CMAKE_CURRENT_BINARY_DIR}"
	cstuff/src)

function (parse_for name info)
  add_executable(parse_${name}
	src/showbuf.c # debugging
	src/output.c
	src/main.c
	src/for_types.c)
  target_link_libraries(parse_${name} PRIVATE libparse)
  get_filename_component(info "${info}" ABSOLUTE BASE_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}")
  target_compile_definitions(parse_${name} PRIVATE
	-DMY_INFO="${info}")
  target_include_directories(parse_${name} PRIVATE
	"${CMAKE_CURRENT_BINARY_DIR}"
	cstuff/src)

endfunction(parse_for)

function(closure_setup_targets header targets)
  if(targets)
	foreach(target IN LISTS targets)
	  target_sources("${target}" PUBLIC "${header}")
	  target_include_directories("${target}" PUBLIC
		"${CMAKE_CURRENT_BINARY_DIR}"
		cstuff/src)
	endforeach(target)
  endif()
endfunction(closure_setup_targets)

function (closure_for name)
  cmake_parse_arguments(PARSE_ARGV 1 A "" "INFO" "TARGETS")
  if(NOT A_INFO)
	set(A_INFO "src/${name}_info.h")
  endif()
  parse_for(${name} ${A_INFO})
  get_filename_component(
	header
	"${name}.h"
	ABSOLUTE
	BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  add_custom_command(
	OUTPUT "${header}"
	COMMAND
	parse_${name} > ${name}.h
	MAIN_DEPENDENCY
	"${A_INFO}"
	DEPENDS
	parse_${name})
  closure_setup_targets("${header}" "${A_TARGETS}")
endfunction(closure_for)

parse_for(script src/script_info.h)
target_sources(parse_script PRIVATE
  src/script_info.c)

function (closure_script name)
  cmake_parse_arguments(PARSE_ARGV 1 A "" "SCRIPT" "TARGETS")
  if(NOT A_SCRIPT)
	set(A_SCRIPT "${name}")
  endif()
  get_filename_component(A_SCRIPT "${A_SCRIPT}" ABSOLUTE BASE_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}")
  get_filename_component(
	header
	"${name}.h"
	ABSOLUTE
	BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")  
  add_custom_command(
	OUTPUT "${header}"
	COMMAND
	parse_script < "${A_SCRIPT}" > "${name}.h"
	MAIN_DEPENDENCY
	"${A_SCRIPT}"
	DEPENDS
	parse_script)
  closure_setup_targets("${header}" "${A_TARGETS}")
endfunction()

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  include(FindPkgConfig)
  pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)
  add_executable(example
	src/example/main.c)
  target_link_libraries(example PkgConfig::GLIB)
  target_include_directories(example PRIVATE cstuff/src)
  target_compile_options(example PUBLIC -fplan9-extensions)
				  
  add_executable(example2
	src/example2.c)
  closure_script(example2
	TARGETS example2
	SCRIPT src/example2.script)

endif()
