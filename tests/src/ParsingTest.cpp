#define FENTON_TESTS_PROJECT Minrzbas
#define FENTON_TESTS_FUNC_NAME parsing
#define FENTON_TESTS_FILE_NAME "tests/parsing.json"
#define FENTON_TESTS_INPUT\
    using namespace Fenton::Minrzbas;\
    using namespace std::string_literals;\
    const json::object& _in = _vPtr->as_object();\
    std::vector<std::string> _args;\
    for (const json::value& v : _in.at("includeDirs").as_array()) {\
        fs::path _path = v.as_string().c_str();\
        if (_path.is_relative())\
            _path = pathInRes("testProject1" / _path);\
        _args.emplace_back("-I" + _path.string());\
    }\
    std::string _fileName = pathInRes(_in.at("file").as_string().c_str()).string();
#define FENTON_TESTS_ERROR
#define FENTON_TESTS_PRINT_INPUT pretty_print(Fenton::getDefaultOutput(), _in);
#define FENTON_TESTS_EXPECTED //json::object _expected = _case.at("out").as_object();
#define FENTON_TESTS_ACTUAL json::object _actual = unitToJSON(_fileName, _args);
#define FENTON_TESTS_UNEQUAL true //_actual != _expected
#define FENTON_TESTS_PRINT_EXPECTED //pretty_print(Fenton::getDefaultOutput(), _expected);
#define FENTON_TESTS_PRINT_ACTUAL pretty_print(Fenton::getDefaultOutput(), _actual);
#define FENTON_TESTS_TEST_NAME "Parsing"

#include <minrzbas/Parser.hpp>

namespace fs = std::filesystem;

#include <TestTempl.hpp>