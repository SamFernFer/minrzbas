#ifndef FENTON_MINRZBAS_THING_HPP
#define FENTON_MINRZBAS_THING_HPP

#include <boost/program_options.hpp>
#include <string>
#include <filesystem>
#include <memory>
#include <vector>

namespace Fenton::Minrzbas {
    void generate(const boost::program_options::variables_map& vm);
}
#endif