#include <Program.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-cpp-macro"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
#include <boost/program_options.hpp>
#include <boost/json.hpp>
#pragma clang diagnostic pop

#include <iostream>

namespace po = boost::program_options;
namespace Fenton::Minrzbas {
    constexpr const char* versionString = "1.0.0";

    int main(int argc, char** argv) {
        po::options_description _desc("Allowed options");
        _desc.add_options()
            ("help,h", "Displays help information.")
            ("version,V", "Displays the program's version.")
        ;
        po::variables_map _vm;
        po::store(po::parse_command_line(argc, argv, _desc), _vm);
        po::notify(_vm);

        if (_vm.count("help")) {
            std::cout << _desc << "\n";
            return 1;
        }
        if (_vm.count("version")) {
            std::cout << versionString << "\n";
            return 1;
        }
        // std::cout << boost::json::array({0, 1, 9}) << std::endl;
        return 0;
    }
}