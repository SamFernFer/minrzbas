#include <utils/JSON.hpp>

namespace Fenton::Minrzbas {
    boost::json::object UnitToJSON(
        const std::string& filePath,
        const std::vector<std::string>& args
    );
}