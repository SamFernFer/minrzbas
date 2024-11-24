#include <utils/JSON.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-cpp-macro"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
#include <boost/program_options.hpp>
#pragma clang diagnostic pop

#include <string>
#include <vector>

namespace Fenton::Minrzbas {
    boost::json::object unitToJSON(
        const std::string& filePath,
        const std::vector<const char*>& args
    );
    struct OptionsDecomposition {
        // The input and output paths. Output might be empty.
        std::string input, output;
        // The vector storing the arguments.
        std::vector<std::string> args;
        // The arguments to be passed to libclang.
        std::vector<const char*> argv;
    };
    // Decomposes the options in the variables_map so that they can be used more easily elsewhere.
    OptionsDecomposition decomposeOptions(const boost::program_options::variables_map& vm);
}