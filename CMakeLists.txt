cmake_minimum_required(VERSION 3.15)
project(Minrzbas VERSION 1.0)

if(FENTON_BUILD_TESTS)
	add_subdirectory("${PROJECT_SOURCE_DIR}/src")
	set_target_properties(minrzbas PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
		"${PROJECT_SOURCE_DIR}/tests/lib"
	)
	add_subdirectory("${PROJECT_SOURCE_DIR}/tests/src")
else()
	add_subdirectory("${PROJECT_SOURCE_DIR}/src")
	set_target_properties(minrzbas PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
		"${PROJECT_SOURCE_DIR}/lib"
	)
endif()