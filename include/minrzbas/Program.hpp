// This is the code which handles the processing of command-line arguments.
#ifndef FENTON_MINRZBAS_PROGRAM_HPP
#define FENTON_MINRZBAS_PROGRAM_HPP

#include <boost/program_options.hpp>

namespace Fenton::Minrzbas {
    // Gets the list of named options.
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