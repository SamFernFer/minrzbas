#include <utils/FileUtils.hpp>
#include <utils/Misc.hpp>
#include <Common.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <format>

namespace json = boost::json;
namespace fs = std::filesystem;

namespace Fenton::Minrzbas::Tests {
    // file is just the file's name as the rest of the path is already known.
    boost::json::value loadJSONFile(const std::filesystem::path& fileName) {
        const fs::path _filePath = 
            fs::path(FileUtils::getExecutablePath())
            .parent_path()
            .parent_path()
            / "resources"
            / fileName
        ;

        if (!fs::is_regular_file(_filePath))
            throw std::runtime_error(std::format(
                "The file {} does not exist.", quote(_filePath.generic_string())
            ));

        std::fstream _file;
        _file.exceptions(std::ios::failbit | std::ios::badbit);
        _file.open(_filePath);

        std::stringstream _ss;
        _ss << _file.rdbuf();

        json::parse_options _opts;
        _opts.allow_comments = true;
        _opts.allow_trailing_commas = true;

        return json::parse(_ss.str(), {}, _opts);
    }
}