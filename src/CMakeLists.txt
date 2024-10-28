set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_library(minrzbas STATIC
	"${PROJECT_SOURCE_DIR}/external/whereami/src/whereami.c"
	"utils/FileUtils.cpp"
	"utils/JSON.cpp"
	"utils/Misc.cpp"
	"Program.cpp"
	"Thing.cpp"
	"Parser.hpp"
)
# set_property(TARGET minrzbas PROPERTY
#   MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_compile_definitions(minrzbas PUBLIC
	"-DBOOST_ALL_NO_LIB"
	# NOTE: Does it make the program completely static?
	"-DBUILD_SHARED_LIBS=OFF"
	"-DHAVE_SNPRINTF"
)

if(MSVC)
	target_compile_options(minrzbas PUBLIC /Zc:preprocessor)
# else(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
else()
	# target_compile_options(minrzbas -O0)
	# target_compile_options(minrzbas -debug)
	target_compile_options(minrzbas PUBLIC
		-Wpedantic
		-Werror=return-type
		-Wno-switch
		#-Wno-everything
		-Werror=extern-initializer
		-Werror=microsoft-template
	)
endif()

target_include_directories(minrzbas PUBLIC
	"${BOOST_ROOT}"
	"${PYTHON_INCLUDE}"
	"${LIBCLANG_INCLUDE}"
    "${PROJECT_SOURCE_DIR}/external/whereami/src"
    "${PROJECT_SOURCE_DIR}/include"
)
target_link_directories(minrzbas PUBLIC
	"${BOOST_LIB}"
	"${PYTHON_LIB}"
	"${LIBCLANG_LIB}"
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(BOOST_TAGS mt-sgd)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	set(BOOST_TAGS mt-s)
else()
	message(FATAL_ERROR "Unknown build type \"${CMAKE_BUILD_TYPE}\".")
endif()

set(BOOST_VER 1_82)
set(BOOST_TOOLSET clangw17)

target_link_libraries(minrzbas
	"libclang.lib"
             "libboost_chrono-${BOOST_TOOLSET}-${BOOST_TAGS}-x64-${BOOST_VER}.lib"
	           "libboost_json-${BOOST_TOOLSET}-${BOOST_TAGS}-x64-${BOOST_VER}.lib"
	         "libboost_locale-${BOOST_TOOLSET}-${BOOST_TAGS}-x64-${BOOST_VER}.lib"
	"libboost_program_options-${BOOST_TOOLSET}-${BOOST_TAGS}-x64-${BOOST_VER}.lib"
	       "libboost_python38-${BOOST_TOOLSET}-${BOOST_TAGS}-x64-${BOOST_VER}.lib"
	          "libboost_regex-${BOOST_TOOLSET}-${BOOST_TAGS}-x64-${BOOST_VER}.lib"
)

if (NOT FENTON_BUILD_TESTS)
	# TODO: When not building the tests, build an interface executable linked with the 
	# minrzbas library. Remember to change the OUTPUT_NAME property for the executable 
	# target.
	add_executable(program "EntryPoint.cpp")
	set_target_properties(program PROPERTIES RUNTIME_OUTPUT_DIRECTORY
		"${PROJECT_SOURCE_DIR}/bin"
	)
	set_target_properties(program PROPERTIES OUTPUT_NAME "minrzbas")

	target_compile_definitions(program PUBLIC
		"-DBOOST_ALL_NO_LIB"
		"-DBUILD_SHARED_LIBS=OFF"
		"-DHAVE_SNPRINTF"
	)

	target_include_directories(program PUBLIC "${PROJECT_SOURCE_DIR}/include")
	# Link with the library containing the actual code.
	target_link_libraries(program PUBLIC minrzbas)
endif()