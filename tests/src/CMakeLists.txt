set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

add_executable(tests
	"Program.cpp"
	"Common.cpp"
	"ParsingTest.cpp"
	"TemplAccessTest.cpp"
	"CurrentType.cpp"
)

target_compile_definitions(tests PUBLIC
)

if(MSVC)
	target_compile_options(tests PRIVATE
		/Zc:preprocessor
	)
else()
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

target_include_directories(tests SYSTEM PRIVATE
	"${BOOST_ROOT}"
)
target_include_directories(tests PRIVATE
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/tests/include"
)
target_link_libraries(tests
	minrzbas
)
set_target_properties(tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY
	"${PROJECT_SOURCE_DIR}/tests/bin"
)