#ifndef FENTON_MINRZBAS_THING_HPP
#define FENTON_MINRZBAS_THING_HPP

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-cpp-macro"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
#include <boost/program_options.hpp>
#pragma clang diagnostic pop
#include <string>
#include <filesystem>
#include <memory>
#include <vector>

namespace Fenton::Minrzbas {
    void generate(const boost::program_options::variables_map& vm);
}
#endif