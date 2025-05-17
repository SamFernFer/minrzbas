# minrzbas
A utility program created to make introspecting C++ code easier. Generates a JSON representation of the entities in and directly and indirectly included non-system files for a C++ file. Use `--input` to specify the file to be introspected, `-I` or `--include` to include a normal file, `--isystem` to include a system header and `-D` to define a macro. For best results, it is recommended to include all files as well as defining every macro which would be used when compiling the file. Consult the tests or the `--help` command for help.

# Compilation

Follow the normal CMake compilation steps, but remember to define the following variables:

- `BOOST_ROOT`: Boost installation;
- `BOOST_LIB`: directory containing the compiled Boost libraries;
- `LIBCLANG_INCLUDE`: LLVM include directory;
- `LIBCLANG_LIB`: LLVM lib directory;
- `FENTON_BUILD_TESTS`: set to `ON` to build the tests;

# Limitations

- Templates are not implemented.
- Attributes others than `clang::annotate` are not included;