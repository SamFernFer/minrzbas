#include <utils/JSON.hpp>

#include <format>
#include <sstream>
#include <fstream>
#include <locale>

namespace Fenton {
    namespace fs = std::filesystem;
    namespace json = boost::json;

    json::value parseFromPath(const fs::path& path) {
        std::ifstream _fs;
        _fs.exceptions(std::ios::badbit | std::ios::failbit);

        if (!fs::is_regular_file(path))
            throw std::runtime_error(std::format("The file \"{0}\" could not be found.", path.string()));

        //try {
        _fs.open(path);
        json::parse_options _opts;
        _opts.allow_comments = true;
        _opts.allow_trailing_commas = true;

        std::stringstream _ss;
        _ss << _fs.rdbuf();

        return json::parse(_ss.str(), {}, _opts);
        /*}
        catch (std::exception& e) {
            std::cerr << "Failed to load the file \"" << path << "\".";
            std::cerr << e.what() << std::endl;
        }
        return json::value();*/
    }

    void pretty_print(std::ostream& os, json::value const& jv, std::string* indent) {
		
        std::string indent_;
        if (!indent)
            indent = &indent_;
        switch (jv.kind())
        {
        case json::kind::object:
        {
            auto const& obj = jv.get_object();
            if (obj.empty()) {
                os << "{}";
            } else {
                os << "{\n";
                indent->append(4, ' ');
                auto it = obj.begin();
                for (;;)
                {
                    os << *indent << json::serialize(it->key()) << " : ";
                    pretty_print(os, it->value(), indent);
                    if (++it == obj.end())
                        break;
                    os << ",\n";
                }
                os << "\n";
                indent->resize(indent->size() - 4);
                os << *indent << "}";
            }
            break;
        }

        case json::kind::array:
        {
            auto const& arr = jv.get_array();
            if (arr.empty()) {
                os << "[]";
            } else {
                os << "[\n";
                indent->append(4, ' ');
                
                auto it = arr.begin();
                for (;;)
                {
                    os << *indent;
                    pretty_print(os, *it, indent);
                    if (++it == arr.end())
                        break;
                    os << ",\n";
                }
                os << "\n";
                indent->resize(indent->size() - 4);
                os << *indent << "]";
            }
            break;
        }

        case json::kind::string:
        {
            os << json::serialize(jv.get_string());
            break;
        }

        case json::kind::uint64:
            os << jv.get_uint64();
            break;

        case json::kind::int64:
            os << jv.get_int64();
            break;

        case json::kind::double_:
            os << jv.get_double();
            break;

        case json::kind::bool_:
            if (jv.get_bool())
                os << "true";
            else
                os << "false";
            break;

        case json::kind::null:
            os << "null";
            break;
        }

        if (indent->empty())
            os << "\n";
    }

    std::string pretty_serialise(json::value const& jv) {
        std::stringstream _ss;
        pretty_print(_ss, jv);
        return _ss.str();
    }
}