cmake_minimum_required(VERSION 3.5)
project(Caknag VERSION 1.0)

if(FENTON_BUILD_TESTS)
	add_subdirectory("${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/tests/caknag_build")
	set_target_properties(caknag PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
		"${PROJECT_SOURCE_DIR}/tests/lib"
	)
	add_subdirectory("${PROJECT_SOURCE_DIR}/tests/src")
else()
	add_subdirectory("${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/build")
	set_target_properties(caknag PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
		"${PROJECT_SOURCE_DIR}/lib"
	)

	# TODO: When not building the tests, build an interface executable linked with the 
	# caknag library. Remember to change the OUTPUT_NAME property for the executable 
	# target.
	# add_executable(program "Program.cpp")
endif()