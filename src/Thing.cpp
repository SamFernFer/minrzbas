#include <minrzbas/Thing.hpp>
#include <utils/Misc.hpp>
#include <iostream>
#include <utility>
#include <filesystem>

namespace fs = std::filesystem;
namespace po = boost::program_options;
namespace Fenton::Minrzbas {
    std::ostream& operator<<(std::ostream& stream, const std::vector<std::string>& vec) {
        if (!vec.empty()) {
            stream << quote(vec.front());
            for (auto it = vec.cbegin()+1; it != vec.cend(); ++it) {
                stream << ",\n" << quote(*it);
            }
        }
        return stream;
    }
    static std::pair<std::string,std::string> splitOnce(
        std::string_view str,
        const char sep
    ) {
        std::pair<std::string,std::string> _pair;
        for (std::size_t i = 0; i < str.size(); ++i) {
            // Split by the character.
            if ((str[i] == sep)) {
                bool _isNotLast = i < (str.size() - 1);
                // Allows escaping the separator character by doubling it.
                if (_isNotLast && str[i + 1] == sep) {
                    ++i;
                    continue;
                }
                // The first half.
                _pair.first = std::string(str.substr(0, i));
                // Prevents an error inside substr when the separator character is the last one.
                if (_isNotLast) {
                    // Retrieves the second half.
                    _pair.second = std::string(str.substr(i + 1));
                }
                return _pair;
            }
        }
        // There is no separator character, so there's no second half.
        _pair.first = std::string(str);
        return _pair;
    }
    void addDirectory(
        Context& ctx,
        std::string_view class_,
        std::string_view path,
        std::string_view cond
    ) {
        Class _class {
            .class_ = std::string(class_),
            .cond = std::string(cond)
        };
        // TODO: change to a heterogeneous unordered_map.
        if (auto _find = ctx.dirs.find(std::string(path)); _find == ctx.dirs.cend()) {
            ctx.dirs.emplace(std::make_pair(std::string(path), std::list<Class>({_class})));
        } else {
            _find->second.emplace_back(_class);
        }
    }
    Context getContext(const po::variables_map& vm) {
        Context _ctx;
        if (vm.count("include")) {
            // std::cout << vm["include"].as<std::vector<std::string>>() << std::endl;
            for (const std::string& d : vm["include"].as<std::vector<std::string>>()) {
                std::pair<std::string,std::string> _split = splitOnce(d, ':');
                addDirectory(_ctx, "include", _split.first, _split.second);
            }
        }
        if (vm.count("src")) {
            // std::cout << vm["include"].as<std::vector<std::string>>() << std::endl;
            for (const std::string& d : vm["src"].as<std::vector<std::string>>()) {
                std::pair<std::string,std::string> _split = splitOnce(d, ':');
                addDirectory(_ctx, "src", _split.first, _split.second);
            }
        }
        return std::move(_ctx);
    }
    void doThing(const boost::program_options::variables_map& vm) {
        Context _ctx = getContext(vm);
    }
}