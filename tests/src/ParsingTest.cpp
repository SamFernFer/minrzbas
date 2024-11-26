#define FENTON_TESTS_PROJECT Minrzbas
#define FENTON_TESTS_FUNC_NAME parsing
#define FENTON_TESTS_INIT\
    fs::path _resPath = fs::path(FileUtils::getExecutablePath())\
        .parent_path()\
        .parent_path()\
        / "resources"\
    ;\
    std::unordered_map<std::string, std::string> _formatVars = {\
        { "resDir", _resPath.string() }\
    };\
    std::list<fs::path> _createdFiles;
#define FENTON_TESTS_FILE_NAME "parsing.json"
#define FENTON_TESTS_CASES_INIT
    /*Generates the versions of the test cases with the "--output" option.*/\
#define FENTON_TESTS_INPUT\
    using namespace Fenton::Minrzbas;\
    using namespace std::string_literals;\
    json::object& _in = _vPtr->as_object();\
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
    /*The work directory might change with each case.*/\
    _formatVars["workDir"] = fs::current_path().string();\
    if (const json::value* _cwdPtr = _in.if_contains("cwd")) {\
        _cwdReverter = std::make_unique<CwdReverter>(pathInRes(_cwdPtr->as_string().c_str()));\
    }\
    /*Initialises the arguments vector with an empty string to simulate the first argument.*/\
    std::vector<const char*> _cArgs { "" };\
    json::array& _jsonArgs = _in.at("args").as_array();\
    for (auto it = _jsonArgs.begin(); it != _jsonArgs.end(); ++it) {\
        json::string& _str = it->as_string();\
        /*Modifies the argument itself so that the final arguments are displayed.*/\
        _str = map_format(_str, _formatVars);\
        _cArgs.emplace_back(_str.c_str()); /*Safe, as the string will remain alive.*/\
    }\
    po::options_description _desc = getOptionsDesc();\
    po::positional_options_description _posDesc = getPosOptionsDesc();\
    po::variables_map _vm = getVarMap(_desc, _posDesc, _cArgs.size(), _cArgs.data());\
    OptionsDecomposition _opts = decomposeOptions(_vm);
#define FENTON_TESTS_ERROR
#define FENTON_TESTS_PRINT_INPUT pretty_print(Fenton::getDefaultOutput(), _in);
#define FENTON_TESTS_EXPECTED\
    json::object _expected;\
    fs::path _expectedPath = _resPath / "expected" / _case.at("out").as_string().c_str();\
    if (fs::exists(_expectedPath))\
        _expected = parseFromPath(_expectedPath).as_object();\
        if (json::value* _incs = _expected.if_contains("inclusions")) {\
            json::array& _incsArr = _incs->as_array();\
            /*Converts the relative paths to absolute ones.*/\
            for (auto it = _incsArr.begin(); it != _incsArr.end(); ++it) {\
                *it = fs::absolute(it->as_string().c_str()).generic_string();\
            }\
        }\
    else\
        Fenton::printlnf("[ERROR] The file \"{0}\" could not be found.", _expectedPath.string());
#define FENTON_TESTS_ACTUAL\
    json::object _actual;\
    if (_opts.output.empty())\
        _actual = unitToJSON(_opts.input, _opts.includeDirs, _opts.argv, _opts.dumpAST);\
    else {\
        generate(_opts);\
        fs::path _outPath = fs::canonical(_opts.output);\
        /*Emplaces the output path in canonical form, as the work directory might change.*/\
        _createdFiles.emplace_back(_outPath);\
        _actual = std::move(parseFromPath(_outPath).as_object());\
    }
        
#define FENTON_TESTS_UNEQUAL _actual != _expected
#define FENTON_TESTS_PRINT_EXPECTED pretty_print(Fenton::getDefaultOutput(), _expected);
#define FENTON_TESTS_PRINT_ACTUAL pretty_print(Fenton::getDefaultOutput(), _actual);
#define FENTON_TESTS_FINISH\
    /*Deletes the created files.*/\
    for (const fs::path& p : _createdFiles) {\
        fs::remove(p);\
    }
#define FENTON_TESTS_TEST_NAME "Parsing"

#include <minrzbas/Parser.hpp>
#include <minrzbas/Generator.hpp>
#include <minrzbas/Program.hpp>
#include <utils/FileUtils.hpp>
#include <memory>
#include <list>

namespace fs = std::filesystem;
namespace po = boost::program_options;

#include <TestTempl.hpp>