#include <utils/JSON.hpp>
#include <string>
#include <vector>

namespace Fenton::Minrzbas {
    boost::json::object unitToJSON(
        const std::string& filePath,
        const std::vector<std::string>& args
    );
}