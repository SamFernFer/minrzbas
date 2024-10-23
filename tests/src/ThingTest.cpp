#include <minrzbas/Program.hpp>
#include <minrzbas/Thing.hpp>
#include <Common.hpp>
#include <utils/Misc.hpp>
#include <utils/JSON.hpp>
#include <iostream>

namespace json = boost::json;
namespace Fenton::Minrzbas::Tests {
    static Context contextFromArray(const json::array& arr) {
        Context _ctx;
        for (const json::value& v : arr) {
            addDirectory(
                _ctx,
                v.at(0).as_string().c_str(),
                v.at(1).as_string().c_str(),
                v.at(2).as_string().c_str()
            );
        }
        return _ctx;
    }
    // Cheecks if two maps of string to directories are equal. The values are pointers, 
    // so this function compares the objects pointed instead of their addresses.
    // If any of the values is nullptr, considers them different.
    static bool dirsMapEqual(
        const decltype(Context::dirs)& a,
        const decltype(Context::dirs)& b
    ) {
        // They are for sure different if they have different sizes.
        if (a.size() != b.size()) return false;
        // Copy the first map.
        decltype(Context::dirs) _tempMap = a;
        for (const auto& p : b) {
            // The value is nullptr, so returns false.
            if (!p.second) return false;
            // Checks if the first map contains the key.
            if (const auto& _find = _tempMap.find(p.first); _find != _tempMap.end()) {
                // The value is nullptr, so returns false.
                if (!_find->second) return false;
                // The Directory objects are different, so returns false.
                if (*p.second != *_find->second) return false;
                // Removes the node to make the next searches faster.
                _tempMap.erase(_find);
            } else {
                return false;
            }
        }
        return true;
    }
    static void printCtxDirs(std::ostream& stream, const Context& ctx) {
        stream << "[\n";
        for (const auto& d : ctx.orderedDirs) {
            for (const auto& c : d.classes) {
                stream << '\t'
                    // The class' name.
                    << quote(c.class_)
                    // The path.
                    << ':' << quote(d.path)
                    // The class' condition.
                    << ':' << quote(c.cond)
                    // The trailing comma is not irrelevant.
                    << ",\n"
                ;
            }
        }
        stream << "]\n";
    }
    bool classes() {
        bool _pass = true;

        // Remember not to use a reference, because the original value will get 
        // destroyed.
        const json::array _cases = loadJSONFile("dirs/list.json").as_array();

        std::string_view _title = "?";

        for (std::size_t i = 0; i < _cases.size(); ++i) {
            const json::value& _val = _cases[i];
            // Using a reference is possible because the value on which as_obejct is 
            // called is not temporary.
            const json::object& _case = _val.as_object();

            // Sets the title.
            if (const json::value* _vPtr = _case.if_contains("title"))
                _title = _vPtr->as_string().c_str();
            // This case has a test.
            if (const json::value* _vPtr = _case.if_contains("in")) {
                try {
                    const json::array& _in = _vPtr->as_array();
                    std::vector<const char*> _argsVec;
                    _argsVec.emplace_back("");
                    for (const json::value& v : _in) {
                        _argsVec.emplace_back(v.as_string().c_str());
                    }
                    _argsVec.emplace_back(nullptr);

                    // A C++ exception is expected.
                    if (const json::value* _vPtr = _case.if_contains("exception")) {
                        const json::string& _expectedWhat = 
                            _vPtr->as_string()
                        ;
                        try {
                            // Send the arguments to the parser function.
                            Context _ctx = getContext(getVarMap(
                                getOptionsDesc(), _argsVec.size() - 1, _argsVec.data()
                            ));
                            _pass = false;
                            Fenton::println("[Input] ");
                            pretty_print(Fenton::getDefaultOutput(), _in);
                            Fenton::println("[Expected] exception:");
                            Fenton::println(_expectedWhat.c_str());
                            Fenton::println("[Actual] exception: not thrown");
                            Fenton::printlnf("[CASE {0} FAILED] {1}", i, _title);
                        } catch (const std::exception& e) {
                            // If the expected exception message and the actual one are 
                            // NOT equal.
                            const char* _what = e.what();
                            if (std::strcmp(_what, _expectedWhat.c_str()) != 0) {
                                _pass = false;
                                Fenton::println("[Input] ");
                                pretty_print(Fenton::getDefaultOutput(), _in);
                                Fenton::println("[Expected] exception:");
                                Fenton::println(_expectedWhat.c_str());
                                Fenton::println("[Actual] exception:");
                                Fenton::println(_what);
                                Fenton::printlnf("[CASE {0} FAILED] {1}", i, _title);
                            }
                        } catch (...) {
                            _pass = false;
                            Fenton::println("[Input] ");
                            pretty_print(Fenton::getDefaultOutput(), _in);
                            Fenton::println("[Expected] exception:");
                            Fenton::println(_expectedWhat.c_str());
                            Fenton::println("[Actual] exception: unknown");
                            Fenton::printlnf("[CASE {0} FAILED] {1}", i, _title);
                        }
                    // An output is expected.
                    } else {
                        using namespace Fenton::Minrzbas;

                        Context _expectedCtx = contextFromArray(_case.at("out").as_array());
                        Context _ctx = getContext(getVarMap(
                            getOptionsDesc(), _argsVec.size() - 1, _argsVec.data()
                        ));

                        if (
                            _ctx.orderedDirs != _expectedCtx.orderedDirs // Compares the lists.
                            || !dirsMapEqual(_ctx.dirs, _expectedCtx.dirs) // Compares the maps.
                        ) {
                            _pass = false;
                            Fenton::println("[Input] ");
                            pretty_print(Fenton::getDefaultOutput(), _in);
                            Fenton::println("[Expected] context:");
                            printCtxDirs(Fenton::getDefaultOutput(), _expectedCtx);
                            Fenton::println("[Actual] context:");
                            printCtxDirs(Fenton::getDefaultOutput(), _ctx);
                            Fenton::printlnf("[CASE {0} FAILED] {1}", i, _title);
                        }
                    }
                } catch (const std::exception& e) {
                    _pass = false;
                    Fenton::println("[Exception]");
                    Fenton::println(e.what());
                    Fenton::printlnf("[CASE {0} FAILED] {1}", i, _title);
                }
            }
        }
        Fenton::printlnf("[{}] Classes", _pass? "PASS" : "FAIL");
        return _pass;
    }
}