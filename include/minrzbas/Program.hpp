// This is the code which handles the processing of command-line arguments.
#ifndef FENTON_MINRZBAS_PROGRAM_HPP
#define FENTON_MINRZBAS_PROGRAM_HPP
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-cpp-macro"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
#include <boost/program_options.hpp>
#pragma clang diagnostic pop

namespace Fenton::Minrzbas {
    // Gets the list of options.
    boost::program_options::options_description getOptionsDesc();
    // Gets the list of positinal options to be passed to libclang.
    boost::program_options::positional_options_description getPosOptionsDesc();
    // Parses the command-line options and returns the variables map.
    boost::program_options::variables_map getVarMap(
        const boost::program_options::options_description& desc,
        const boost::program_options::positional_options_description& posDesc,
        int argc,
        const char** argv
    );
    // Handles the command-line options.
    int main(int argc, const char** argv);
}
#endif