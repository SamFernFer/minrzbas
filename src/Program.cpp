#include <minrzbas/Program.hpp>
#include <minrzbas/Thing.hpp>

#include <iostream>

namespace po = boost::program_options;
namespace Fenton::Minrzbas {
    constexpr const char* versionString = "1.0.0";

    po::options_description getOptionsDesc() {
        po::options_description _desc("Allowed options");
        _desc.add_options()
            ("help,h", "Displays help information.")
            ("version,V", "Displays the program's version.")
            ("include,I", po::value<std::vector<std::string>>(),
                "Adds a path to the \"include\" class.")
            ("src,S", po::value<std::vector<std::string>>(),
                "Adds a path to the \"src\" class.")
        ;
        return _desc;
    }
    po::variables_map getVarMap(const po::options_description& desc, int argc, const char** argv) {
        po::variables_map _vm;
        po::store(po::parse_command_line(argc, argv, desc), _vm);
        po::notify(_vm);
        return std::move(_vm);
    }
    int main(int argc, const char** argv) {
        po::options_description _desc = getOptionsDesc();
        // Displays help information when no arguments are passed.
        if (argc <= 1) {
            std::cout << _desc << "\n";
            return 1;
        }
        try {
            po::variables_map _vm = getVarMap(_desc, argc, argv);
            if (_vm.count("help")) {
                std::cout << _desc << "\n";
                return 1;
            }
            if (_vm.count("version")) {
                std::cout << versionString << "\n";
                return 1;
            }
            getContext(_vm);
            return 0;
        } catch (const std::exception& e) {
            std::cout << "[Exception] " << e.what() << "\n\n";
            std::cout << _desc << "\n";
            return -1;
        }
    }
}