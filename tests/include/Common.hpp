#ifndef FENTON_MINRZBAS_TESTS_COMMON_HPP
#define FENTON_MINRZBAS_TESTS_COMMON_HPP

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-cpp-macro"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
#include <boost/json.hpp>
#pragma clang diagnostic pop

#include <utils/JSON.hpp>
#include <filesystem>

namespace Fenton::Minrzbas::Tests {
    // file is just the file's name as the rest of the path is already known.
    boost::json::value loadJSONFile(const std::filesystem::path& fileName);
}

#endif