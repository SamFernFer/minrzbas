#define FENTON_TESTS_PROJECT Minrzbas
#define FENTON_TESTS_FUNC_NAME parsing
#define FENTON_TESTS_FILE_NAME "parsing.json"
#define FENTON_TESTS_INPUT\
    using namespace Fenton::Minrzbas;\
    using namespace std::string_literals;\
    const json::object& _in = _vPtr->as_object();\
    /*Changes the current directory and makes sure it gets reverted back later.*/\
    struct CwdReverter {\
        fs::path _lastWorkDir;\
        CwdReverter(fs::path newPath) {\
            _lastWorkDir = fs::current_path();\
            fs::current_path(newPath);\
        }\
        ~CwdReverter() {\
            fs::current_path(_lastWorkDir);\
        }\
    };\
    std::unique_ptr<CwdReverter> _cwdReverter = nullptr;\
    if (const json::value* _cwdPtr = _in.if_contains("cwd")) {\
        _cwdReverter = std::make_unique<CwdReverter>(pathInRes(_cwdPtr->as_string().c_str()));\
    }\
    /*Builds the argument vector.*/\
    std::vector<std::string> _args;\
    /*Initialises the vector with a c-string in it already, to */\
    std::vector<const char*> _cArgs { "" };\
    for (const json::value& v : _in.at("args").as_array()) {\
        const char* _str = v.as_string().c_str();\
        _args.emplace_back(_str);\
        _cArgs.emplace_back(_str);\
    }\
    po::options_description _desc = getOptionsDesc();\
    po::positional_options_description _posDesc = getPosOptionsDesc();\
    po::variables_map _vm = getVarMap(_desc, _posDesc, _cArgs.size(), _cArgs.data());\
    OptionsDecomposition _opts = decomposeOptions(_vm);
#define FENTON_TESTS_ERROR
#define FENTON_TESTS_PRINT_INPUT pretty_print(Fenton::getDefaultOutput(), _in);
#define FENTON_TESTS_EXPECTED //json::object _expected = _case.at("out").as_object();
#define FENTON_TESTS_ACTUAL json::object _actual = unitToJSON(_opts.input, _opts.argv);
#define FENTON_TESTS_UNEQUAL true //_actual != _expected
#define FENTON_TESTS_PRINT_EXPECTED //pretty_print(Fenton::getDefaultOutput(), _expected);
#define FENTON_TESTS_PRINT_ACTUAL pretty_print(Fenton::getDefaultOutput(), _actual);
#define FENTON_TESTS_TEST_NAME "Parsing"

#include <minrzbas/Parser.hpp>
#include <minrzbas/Program.hpp>
#include <memory>

namespace fs = std::filesystem;
namespace po = boost::program_options;

#include <TestTempl.hpp>