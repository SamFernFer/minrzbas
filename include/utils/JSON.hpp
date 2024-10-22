#ifndef FENTON_JSON_HPP
#define FENTON_JSON_HPP

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
#include <boost/json.hpp>
#pragma clang diagnostic pop

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