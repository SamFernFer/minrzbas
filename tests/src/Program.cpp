#include <utils/Misc.hpp>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-cpp-macro"
#include <boost/locale.hpp>
#pragma clang diagnostic pop
#include <iostream>
#include <locale>

namespace Fenton::Minrzbas::Tests {
    bool classes();
    bool filtering();
    bool parsing();
}
int main() {
    using namespace Fenton::Minrzbas::Tests;

    std::locale::global(boost::locale::generator().generate(".UTF-8"));
    std::ios_base::sync_with_stdio(false);
    // std::locale::global(std::locale(".UTF-8"));

    std::ostream& _out = std::cout;
    std::cout.imbue(std::locale());

    // std::cout << "Não." << std::endl;

    // Changes the default output stream.
    Fenton::setDefaultOutput(_out);

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