#ifndef FENTON_MINRZBAS_TESTS_COMMON_HPP
#define FENTON_MINRZBAS_TESTS_COMMON_HPP

#include <boost/json.hpp>
#include <utils/JSON.hpp>
#include <filesystem>

namespace Fenton::Minrzbas::Tests {
    // Returns the full file path from a relative path inside the resources directory.
    std::filesystem::path pathInRes(const std::filesystem::path& fileName);
    // file is just the file's name as the rest of the path is already known.
    boost::json::value loadJSONFile(const std::filesystem::path& fileName);
}

#endif