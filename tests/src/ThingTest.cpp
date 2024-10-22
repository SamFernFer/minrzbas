#include <minrzbas/Program.hpp>
#include <minrzbas/Thing.hpp>
#include <utils/Misc.hpp>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-cpp-macro"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
#include <boost/json.hpp>
#pragma clang diagnostic pop
#include <iostream>

namespace Fenton::Minrzbas::Tests {
    static Context contextFromArray(const json::array& arr) {
        Context _ctx;
        for (const json::value& v : arr) {
            addDirectory(
                _ctx,
                v.at(0).as_string().c_str(),
                v.at(1).as_string().c_str()
                v.at(2).as_string().c_str()
            );
        }
        return _ctx;
    }
    static print(std::ostream& stream, const Context& ctx) {
        
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
                    std::vector<const char*> _argsVec;
                    _argsVec.emplace_back("");
                    for (const json::value& v : _vPtr->as_array()) {
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
                            Fenton::Minrzbas::main(_argsVec.size()-1, _argsVec.data());
                            _pass = false;
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
                                Fenton::println("[Expected] exception:");
                                Fenton::println(_expectedWhat.c_str());
                                Fenton::println("[Actual] exception:");
                                Fenton::println(_what);
                                Fenton::printlnf("[CASE {0} FAILED] {1}", i, _title);
                            }
                        } catch (...) {
                            _pass = false;
                            Fenton::println("[Expected] exception:");
                            Fenton::println(_expectedWhat.c_str());
                            Fenton::println("[Actual] exception: unknown");
                            Fenton::printlnf("[CASE {0} FAILED] {1}", i, _title);
                        }
                    // An output is expected.
                    } else {
                        Context _expectedCtx = contextFromArray(_case.at("out").as_array());

                        Fenton::
                        Context _ctx = 
                        const Dyn
                            _value = Fenton::Caknag::eval(_in, _env),
                            _expectedValue = dynFromJSONArray(_out)
                        ;

                        if (_value != _expectedValue) {
                            _pass = false;
                            Fenton::printlnf("[Input] {}", quote(_in));
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