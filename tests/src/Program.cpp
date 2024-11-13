#include <utils/Misc.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
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

    boost::locale::generator _gen;
    using facets = boost::locale::category_t;
    using chars = boost::locale::char_facet_t;
    _gen.categories(
        facets::convert
        | facets::collation
        | facets::message
        | facets::codepage
        | facets::boundary
        | facets::information
    );
    _gen.characters(
        // chars::char32_f
        // | chars::char16_f
        chars::wchar_f
        | chars::char_f
    );

    // std::locale::global(std::locale(".UTF-8"));
    std::locale::global(_gen.generate(".UTF-8"));

    std::cout.imbue(std::locale(""));
    std::wcout.imbue(std::locale(""));

    std::ostream& _out = std::cout;

    std::cout << "Não." << std::endl;

    // Changes the default output stream.
    Fenton::setDefaultOutput(_out);

    Fenton::println("Desde já, olá! こんにちは！");
    Fenton::println(2349.92304);

    std::cout << 1123123 << std::endl;

    std::wstring wstr = L"Привет, мир!";  // "Hello, world!" in Russian

    std::wcout << wstr << std::endl;

    std::cin.get();

    return 0;

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