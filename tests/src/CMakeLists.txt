set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_executable(tests
	"Program.cpp"
	"Common.cpp"
	"ThingTest.cpp"
)

target_compile_definitions(tests PUBLIC
	"-DBOOST_ALL_NO_LIB"
	# NOTE: Does it make the program completely static?
	"-DBUILD_SHARED_LIBS=OFF"
	"-DHAVE_SNPRINTF"
)

if(MSVC)
	target_compile_options(tests PRIVATE /Zc:preprocessor)
# else(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
else()
	# target_compile_options(tests -O0)
	# target_compile_options(tests -debug)
	target_compile_options(tests PRIVATE
		--static
		-Wpedantic
		-Werror=return-type
		-Wno-switch
		#-Wno-everything
		-Werror=extern-initializer
		-Werror=microsoft-template
	)
endif()

target_include_directories(tests PRIVATE
	"${BOOST_ROOT}"
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/tests/include"
)
target_link_libraries(tests
	minrzbas
)
set_target_properties(tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY
	"${PROJECT_SOURCE_DIR}/tests/bin"
)