#include <minrzbas/Program.hpp>
#include <minrzbas/Thing.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <iostream>

namespace po = boost::program_options;
namespace Fenton::Minrzbas {
    constexpr const char* versionString = "1.0.0";

    po::options_description getPosOptionsDesc() {
        po::options_description _desc("Allowed options");
        _desc.add_options()
            ("help,h", "Displays help information.")
            ("version,V", "Displays the program's version.")
            ("I", po::value<std::vector<std::string>>(),
                "Adds an include path to be used for dependency-checking and "
                "to be passed as an argument to libclang."
            ),
            (
                "input,i",
                "The input file to instrospect."
            ),
            (
                "output,o",
                "The output file to store the introspected data in JSON format in. "
                "Ommiting it means sending the data to standard output."
            )
        ;
        return _desc;
    }
    po::positional_options_description getOptionsDesc() {
        po::positional_options_description _posDesc;
        _posDesc.add("libclang-arg", -1);
        return _posDesc;
    }
    boost::program_options::variables_map getVarMap(
        const boost::program_options::options_description& desc,
        const boost::program_options::positional_options_description& posDesc,
        int argc,
        const char** argv
    ) {
        po::variables_map _vm;
        po::store(
            po::command_line_parser(argc, argv)
            .options(_desc)
            .positional(_posDesc).run(),
            vm
        );
        po::notify(_vm);
        return std::move(_vm);
    }
    int main(int argc, const char** argv) {

#ifdef _WIN32
        // Uses RAII to make sure the codepage is reset to the previous one before the program 
        // exits.
        const struct CPSetter {
            const decltype(GetConsoleOutputCP()) lastCP;
            // Saves the previous codepage.
            CPSetter() : lastCP(GetConsoleOutputCP()) {
                // Changes the console's codepage to support UTF-8.
                SetConsoleOutputCP(CP_UTF8);
            }
            ~CPSetter() {
                // Reverts the console's codepage to the previous one.
                SetConsoleOutputCP(lastCP);
            }
        } _cpSetter;
#endif

        po::options_description _desc = getOptionsDesc();
        po::positinal_options_description _posDesc = getPosOptionsDesc();
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
            if (vm.count("input") == 0) {
                std::cout << "Missing \"input\" option." << std::endl;
                return -1;
            }
            thing(_vm);
            return 0;
        } catch (const std::exception& e) {
            std::cout << "[Exception] " << e.what() << "\n\n";
            std::cout << _desc << "\n";
            return -1;
        }
    }
}