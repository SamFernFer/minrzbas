#ifndef FENTON_JSON_HPP
#define FENTON_JSON_HPP

#include <boost/json.hpp>
#include <filesystem>
#include <iostream>
#include <string>

namespace Fenton {
    boost::json::value parseFromPath(const std::filesystem::path& path);
    // Serialises the JSON value to the stream.
    void pretty_print(std::ostream& os, boost::json::value const& jv, std::string* indent = nullptr);
    // Returns a string containing the serialised JSON value.
    std::string pretty_serialise(boost::json::value const& jv);
}

#endif