#include <minrzbas/Thing.hpp>
#include <minrzbas/Program.hpp>

#include <utils/JSON.hpp>
#include <utils/Misc.hpp>

#include <iostream>
#include <map>
#include <set>
#include <set>
#include <string>
#include <filesystem>

namespace json = boost::json;
namespace fs = std::filesystem;

#define FENTON_TESTS_PROJECT Minrzbas
#define FENTON_TESTS_FUNC_NAME filtering
#define FENTON_TESTS_FILE_NAME "dirs/filtering.json"
#define FENTON_TESTS_INPUT\
    using namespace Fenton::Minrzbas;/*Aliasing the namespace.*/\
    const json::array& _in = _vPtr->as_array();\
    std::vector<const char*> _argsVec; /*Prepares the command-line arguments.*/\
    _argsVec.emplace_back("");\
    for (const json::value& v : _in) {\
        _argsVec.emplace_back(v.as_string().c_str());\
    }\
    _argsVec.emplace_back(nullptr);\
    Context _ctx = getContext(getVarMap( /*Creates the context.*/\
        getOptionsDesc(), _argsVec.size() - 1, _argsVec.data()\
    ));

#define FENTON_TESTS_ERROR
#define FENTON_TESTS_PRINT_INPUT pretty_print(Fenton::getDefaultOutput(), _in);
#define FENTON_TESTS_EXPECTED\
    std::unordered_map<std::string, std::set<std::filesystem::path>> _expectedOutClasses;\
    outClassesFromJSONObject(_case.at("out").as_object(), _expectedOutClasses);
#define FENTON_TESTS_ACTUAL\
    std::unordered_map<std::string, std::set<std::filesystem::path>> _actualOutClasses;\
    filterFiles(_ctx, _actualOutClasses);
// For testing.
#define FENTON_TESTS_UNEQUAL true
#define FENTON_TESTS_PRINT_EXPECTED printOutClasses(Fenton::getDefaultOutput(), _expectedOutClasses);

#define FENTON_TESTS_PRINT_ACTUAL printOutClasses(Fenton::getDefaultOutput(), _actualOutClasses);
#define FENTON_TESTS_TEST_NAME "Filtering"

namespace Fenton::FENTON_TESTS_PROJECT::Tests {

    void outClassesFromJSONObject(
        const json::object& obj,
        std::unordered_map<std::string, std::set<std::filesystem::path>>& outClasses
    ) {
        for (const auto& p : obj) {
            std::set<std::filesystem::path> _set;
            auto _empl = outClasses.emplace(p.key_c_str(), _set);
            for (const auto& e : p.value().as_array()) {
                _set.emplace(e.as_string().c_str());
            }
        }
    }
    void printPathSet(
        std::ostream& out,
        const std::set<std::filesystem::path>& paths
    ) {
        out << "[";
        for (const auto& p : paths) {
            out << "\t\t" << p << ",\n";
        }
        out << "\t]";
    }
    void printOutClasses(
        std::ostream& out,
        const std::unordered_map<std::string, std::set<std::filesystem::path>>& outClasses
    ) {
        std::map<std::string, std::set<std::filesystem::path>>
            _orderedOutClasses;
        
        // Constructs an ordered version of the classes and its paths.
        for (const auto& c : outClasses) {
            auto _empl = _orderedOutClasses.emplace(c.first, std::set<std::filesystem::path>{});
            auto& _set = _empl.first->second;
            for (const auto& p : c.second) {
                _set.emplace(p);
            }
        }

        // Prints the ordered classes and its paths.
        out << "{\n";
        for (const auto& c : _orderedOutClasses) {
            out << "\t" << quote(c.first);
            printPathSet(out, c.second);
            out << ",\n";
        }
        out << "}\n";
    }
}

#include <TestTempl.hpp>