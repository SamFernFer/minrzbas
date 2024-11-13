set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

add_library(minrzbas STATIC
	"${PROJECT_SOURCE_DIR}/external/whereami/src/whereami.c"
	"utils/FileUtils.cpp"
	"utils/JSON.cpp"
	"utils/Misc.cpp"
	"Program.cpp"
	"Thing.cpp"
	"Parser.cpp"
)

target_compile_definitions(minrzbas PUBLIC
)

if(MSVC)
	target_compile_options(minrzbas PUBLIC /Zc:preprocessor)
else()
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
	"${LIBCLANG_INCLUDE}"
    "${PROJECT_SOURCE_DIR}/external/whereami/src"
    "${PROJECT_SOURCE_DIR}/include"
)
target_link_directories(minrzbas PUBLIC
	"${BOOST_LIB}"
	"${LIBCLANG_LIB}"
)
target_link_libraries(minrzbas PUBLIC
	libclang.lib
)

if (NOT FENTON_BUILD_TESTS)
	add_executable(program "EntryPoint.cpp")
	set_target_properties(program PROPERTIES RUNTIME_OUTPUT_DIRECTORY
		"${PROJECT_SOURCE_DIR}/bin"
	)
	set_target_properties(program PROPERTIES OUTPUT_NAME "minrzbas")

	target_compile_definitions(program PUBLIC
	)

	target_include_directories(program PUBLIC "${PROJECT_SOURCE_DIR}/include")
	# Link with the library containing the actual code.
	target_link_libraries(program PUBLIC minrzbas)
endif()