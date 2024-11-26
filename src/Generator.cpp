#include <minrzbas/Generator.hpp>
#include <minrzbas/Program.hpp>
#include <minrzbas/Parser.hpp>
#include <utils/Misc.hpp>
#include <utils/JSON.hpp>

#include <iostream>
#include <utility>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;
namespace po = boost::program_options;
namespace json = boost::json;

namespace Fenton::Minrzbas {
    void generate(const boost::program_options::variables_map& vm) {
        // Decomposes the options.
        OptionsDecomposition _opts = decomposeOptions(vm);
        generate(_opts);
    }
    void generate(const OptionsDecomposition& opts) {
        std::ofstream _file;
        // Only creates the file if the output path is not empty.
        if (!opts.output.empty()) {
            _file.exceptions(std::ios::badbit | std::ios::failbit);
            _file.open(opts.output);
        }
        // NOTE: For now, only generating a JSON AST is supported.
        json::object _obj = unitToJSON(opts.input, opts.includeDirs, opts.argv);
        if (opts.output.empty()) {
            pretty_print(Fenton::getDefaultOutput(), _obj);
        } else {
            // Writes the data to the file.
            pretty_print(_file, _obj);
        }
    }
}