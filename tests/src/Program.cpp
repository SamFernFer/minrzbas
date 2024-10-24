#include <utils/Misc.hpp>
#include <iostream>

namespace Fenton::Minrzbas::Tests {
    bool classes();
    bool filtering();
}
int main() {
    using namespace Fenton::Minrzbas::Tests;

    std::ostream& _out = std::cout;
    // Changes the default output stream.
    Fenton::setDefaultOutput(_out);

    bool _pass = true;
    try {
        _pass &= classes();
        _pass &= filtering();
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