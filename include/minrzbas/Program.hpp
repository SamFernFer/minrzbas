// This is the code which handles the processing of command-line arguments.

namespace Fenton::Minrzbas {
    // Gets the list of options.
    po::options_description getOptionsDesc();
    po::variables_map getVarMap(int argc, const char** argv);
    // Handles the command-line options.
    int main(int argc, const char** argv);
}