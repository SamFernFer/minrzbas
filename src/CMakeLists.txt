set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# Removes the "-D_DLL" flag, thus linking the program statically with the standard library.
	string(REPLACE "-D_DLL" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
	string(REPLACE "-D_DLL" "" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	# Removes the "-D_DLL" flag, thus linking the program statically with the standard library.
	string(REPLACE "-D_DLL" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
	string(REPLACE "-D_DLL" "" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
else()
	message(FATAL_ERROR "Unknown build type \"${CMAKE_BUILD_TYPE}\"")
endif()

add_library(minrzbas STATIC
	"${PROJECT_SOURCE_DIR}/external/whereami/src/whereami.c"
	"Program.cpp"
)
# set_property(TARGET minrzbas PROPERTY
#   MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_compile_definitions(minrzbas PUBLIC "-DBOOST_ALL_NO_LIB")

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
    "${PROJECT_SOURCE_DIR}/external/whereami/src"
    "${PROJECT_SOURCE_DIR}/include"
)
target_link_directories(minrzbas PUBLIC
	"${BOOST_LIB}"
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# Removes the "-D_DLL" flag, thus linking the program statically with the standard library.
	string(REPLACE "-D_DLL" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
	target_link_libraries(minrzbas
		"libboost_chrono-clangw17-mt-sgd-x64-1_82.lib"
		"libboost_json-clangw17-mt-sgd-x64-1_82.lib"
		"libboost_locale-clangw17-mt-sgd-x64-1_82.lib"
		"libboost_program_options-clangw17-mt-sgd-x64-1_82.lib"
		"libboost_python38-clangw17-mt-sgd-x64-1_82.lib"
		"libboost_regex-clangw17-mt-sgd-x64-1_82.lib"
	)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	# Removes the "-D_DLL" flag, thus linking the program statically with the standard library.
	string(REPLACE "-D_DLL" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})

	# NOTE: Does not work.
	# set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} /MT)

	target_link_libraries(minrzbas
		"libboost_chrono-clangw17-mt-s-x64-1_82.lib"
		"libboost_json-clangw17-mt-s-x64-1_82.lib"
		"libboost_locale-clangw17-mt-s-x64-1_82.lib"
		"libboost_program_options-clangw17-mt-s-x64-1_82.lib"
		"libboost_python38-clangw17-mt-s-x64-1_82.lib"
		"libboost_regex-clangw17-mt-s-x64-1_82.lib"
	)
endif()

if (NOT FENTON_BUILD_TESTS)
	# TODO: When not building the tests, build an interface executable linked with the 
	# minrzbas library. Remember to change the OUTPUT_NAME property for the executable 
	# target.
	add_executable(program "EntryPoint.cpp")
	set_target_properties(program PROPERTIES RUNTIME_OUTPUT_DIRECTORY
		"${PROJECT_SOURCE_DIR}/bin"
	)
	set_target_properties(program PROPERTIES OUTPUT_NAME "minrzbas")

	target_compile_definitions(program PUBLIC "-DBOOST_ALL_NO_LIB" "-DBUILD_SHARED_LIBS=OFF")

	target_include_directories(program PUBLIC "${PROJECT_SOURCE_DIR}/include")
	# Link with the library containing the actual code.
	target_link_libraries(program PUBLIC minrzbas)
endif()