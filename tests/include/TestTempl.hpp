/*
 * Template file for a single test section.
 * You should declare the function called FENTON_TESTS_FUNC_NAME in the caller context and 
 * define the following macros before including this template in the global scope in the test 
 * source file (note that FENTON_TESTS_PROJECT should ideally be the same for all tests for the 
 * same project):

FENTON_TESTS_PROJECT
FENTON_TESTS_FUNC_NAME
FENTON_TESTS_FILE_NAME
FENTON_TESTS_INPUT
FENTON_TESTS_ERROR
FENTON_TESTS_PRINT_INPUT
FENTON_TESTS_EXPECTED
FENTON_TESTS_ACTUAL
FENTON_TESTS_UNEQUAL
FENTON_TESTS_PRINT_EXPECTED
FENTON_TESTS_PRINT_ACTUAL
FENTON_TESTS_TEST_NAME
*/

#include <Common.hpp>
#include <utils/Misc.hpp>

namespace json = boost::json;
namespace Fenton::FENTON_TESTS_PROJECT::Tests {
    bool FENTON_TESTS_FUNC_NAME() {
        bool _pass = true;

        // Remember not to use a reference, because the original value will get 
        // destroyed.
        const json::array _cases = loadJSONFile(FENTON_TESTS_FILE_NAME).as_array();

        std::string_view _title = "?";

        for (std::size_t i = 0; i < _cases.size(); ++i) {
            const json::value& _val = _cases[i];
            // Using a reference to avoid copying.
            const json::object& _case = _val.as_object();

            // Sets the title.
            if (const json::value* _vPtr = _case.if_contains("title"))
                _title = _vPtr->as_string().c_str();
            // This case has a test.
            if (const json::value* _vPtr = _case.if_contains("in")) {
                try {
                    FENTON_TESTS_INPUT

                    // A C++ exception is expected.
                    if (const json::value* _vPtr = _case.if_contains("exception")) {
                        const json::string& _expectedWhat = 
                            _vPtr->as_string()
                        ;
                        try {
                            // Send the arguments to the parser function.
                            FENTON_TESTS_ERROR
                            _pass = false;
                            Fenton::println("[Input] ");
                            FENTON_TESTS_PRINT_INPUT
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
                                FENTON_TESTS_PRINT_INPUT
                                Fenton::println("[Expected] exception:");
                                Fenton::println(_expectedWhat.c_str());
                                Fenton::println("[Actual] exception:");
                                Fenton::println(_what);
                                Fenton::printlnf("[CASE {0} FAILED] {1}", i, _title);
                            }
                        } catch (...) {
                            _pass = false;
                            Fenton::println("[Input] ");
                            FENTON_TESTS_PRINT_INPUT
                            Fenton::println("[Expected] exception:");
                            Fenton::println(_expectedWhat.c_str());
                            Fenton::println("[Actual] exception: unknown");
                            Fenton::printlnf("[CASE {0} FAILED] {1}", i, _title);
                        }
                    // An output is expected.
                    } else {
                        FENTON_TESTS_EXPECTED
                        FENTON_TESTS_ACTUAL

                        if (
                            FENTON_TESTS_UNEQUAL
                        ) {
                            _pass = false;
                            Fenton::println("[Input]");
                            FENTON_TESTS_PRINT_INPUT
                            Fenton::println("[Expected]");
                            FENTON_TESTS_PRINT_EXPECTED
                            Fenton::println("[Actual]");
                            FENTON_TESTS_PRINT_ACTUAL
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
        Fenton::printlnf("[{}] " FENTON_TESTS_TEST_NAME, _pass? "PASS" : "FAIL");
        return _pass;
    }
}