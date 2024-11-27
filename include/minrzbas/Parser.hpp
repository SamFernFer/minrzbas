#include <utils/JSON.hpp>

#include <boost/program_options.hpp>
#include <string>
#include <vector>

namespace Fenton::Minrzbas {
    boost::json::object unitToJSON(
        const std::string& filePath,
        const std::vector<std::string>& includeDirs,
        const std::vector<const char*>& args,
        bool dumpAST
    );
    struct OptionsDecomposition {
        // The input and output paths. Output might be empty.
        std::string input, output;
        // The include directories.
        std::vector<std::string> includeDirs;
        // The vector storing the arguments.
        std::vector<std::string> args;
        // The arguments to be passed to libclang.
        std::vector<const char*> argv;
        // Whether the AST should be displayed before the output.
        bool dumpAST;
    };
    enum class RecordType {
        Class, Struct, Union
    };
    // Decomposes the options in the variables_map so that they can be used more easily elsewhere.
    OptionsDecomposition decomposeOptions(const boost::program_options::variables_map& vm);
}