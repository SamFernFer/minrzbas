#include <minrzbas/Program.hpp>
#include <minrzbas/Thing.hpp>

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
                "The output JSON file to generate from the input file's instrospected data."
                "I ommited, equals the standard output."
            ),
            (
                "input-list",
                "A list of files in JSON format. The JSON should be an array, with each field's "
                "name being a file's full path, with its value the file's modification date."
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
            // getContext(_vm);
            return 0;
        } catch (const std::exception& e) {
            std::cout << "[Exception] " << e.what() << "\n\n";
            std::cout << _desc << "\n";
            return -1;
        }
    }
}