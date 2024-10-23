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
        const Class _class {
            .class_ = std::string(class_),
            .cond = std::string(cond)
        };
        if (auto _find = ctx.dirs.find(path); _find == ctx.dirs.cend()) {
            Directory _dir {
                .path = std::string(path),
                .classes = { _class }
            };
            ctx.orderedDirs.emplace_back(
                _dir
            );
            ctx.dirs.emplace(std::make_pair(
                // The string_view should point to the path variable in the Directory object 
                // in the list. The parameter path is going to be destroyed later.
                std::string_view(ctx.orderedDirs.rbegin()->path),
                ctx.orderedDirs.rbegin().base()
            ));
        } else {
            // Adds the class to the directory's list of classes.
            _find->second->classes.emplace_back(_class);
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
        // Iterate through all directories.
        // - Iterate through all files.
        // - - Iterate through all the classes in this directory.
        // - - - Filter based on the condition and execute each event based on the database of 
        // - - - the previous filtering.
    }
}