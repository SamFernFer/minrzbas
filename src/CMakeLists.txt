set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

add_library(caknag STATIC
    "${PROJECT_SOURCE_DIR}/external/whereami/src/whereami.c"
    "Misc.cpp"
    "JSON.cpp"
    "FileUtils.cpp"
	"Caknag.cpp"
	"Type.cpp"
)

target_compile_definitions(caknag PRIVATE "-DBOOST_ALL_NO_LIB")

if(MSVC)
	target_compile_options(caknag PRIVATE /Zc:preprocessor)
# else(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
else()
	# target_compile_options(caknag -O0)
	# target_compile_options(caknag -debug)
	target_compile_options(caknag PRIVATE
		-Wpedantic
		-Werror=return-type
		-Wno-switch
		#-Wno-everything
		-Werror=extern-initializer
		-Werror=microsoft-template
	)
endif()

target_include_directories(caknag PRIVATE
    "${BOOST_ROOT}"
    "${PROJECT_SOURCE_DIR}/external/whereami/src"
    "${PROJECT_SOURCE_DIR}/include"
)