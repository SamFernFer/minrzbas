#include <utils/Misc.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <iostream>
#include <locale>

namespace Fenton::Minrzbas::Tests {
    bool classes();
    bool filtering();
    bool parsing();
}
int main() {
    using namespace Fenton::Minrzbas::Tests;

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    // Changes the default output stream (we're using std::cout for the tests).
    // Fenton::setDefaultOutput(_out);

    bool _pass = true;
    try {
        // _pass &= classes();
        // _pass &= filtering();
        _pass &= parsing();
    } catch(const std::exception& e) {
        _pass = false;
        Fenton::printlnf("[EXCEPTION]\n{}", e.what());
    } catch(...) {
        _pass = false;
        Fenton::printlnf("[UNKNOWN EXCEPTION]");
    }
    Fenton::printlnf("[FINAL]: {}", _pass? "PASS" : "FAIL");
    std::cin.get();
    return 0;
}