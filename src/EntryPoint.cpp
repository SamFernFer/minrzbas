// This is the actual entry point to the program.

#include <minrzbas/Program.hpp>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-cpp-macro"
#include <boost/locale.hpp>
#pragma clang diagnostic pop
#include <locale>

int main(int argc, const char** argv) {
    std::locale::global(boost::locale::generator().generate(".UTF-8"));
    return Fenton::Minrzbas::main(argc, argv);
}