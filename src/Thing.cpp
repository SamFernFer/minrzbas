#include <minrzbas/Thing.hpp>
#include <utils/Misc.hpp>

#define BOOST_PYTHON_STATIC_LIB
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-cpp-macro"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
#include <boost/python.hpp>
#pragma clang diagnostic pop

#include <iostream>
#include <utility>
#include <filesystem>

namespace fs = std::filesystem;
namespace po = boost::program_options;
namespace py = boost::python;

namespace Fenton::Minrzbas {
    static bool _pyInit = false;
    void initPy() {
        Py_Initialize();
        _pyInit = true;
    }
    void tryInitPy() {
        if (!_pyInit) {
            initPy();
        }
    }

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
        if (auto _find = ctx.dirs.find(std::string(path)); _find == ctx.dirs.cend()) {
            Directory _dir {
                .path = std::string(path),
                .classes = { _class }
            };
            Directory& _dirRef = ctx.orderedDirs.emplace_back(_dir);
            ctx.dirs.emplace(std::make_pair(std::string(path), &_dirRef));
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
        if (vm.count("srcdir")) {
            _ctx.srcdir = vm["srcdir"].as<std::string>();
        }
        if (vm.count("resdir")) {
            _ctx.resdir = vm["resdir"].as<std::string>();
        }
        return std::move(_ctx);
    }
    void filterDir(
        const std::string_view root,
        const std::string relName,
        const std::multimap<std::string, py::object>& condsMap,
        std::unordered_map<std::string, std::list<std::filesystem::path>>& outClasses
    ) {
        for (auto it = fs::directory_iterator(path); it != fs::directory_iterator(); it++) {
            if 
            for (const auto& p : condsMap) {
                p.second(relName);
            }
        }
    }
    void filterFiles(
        const Context& ctx,
        std::unordered_map<std::string, std::list<std::filesystem::path>>& outClasses
    ) {
        tryInitPy();
        for (const auto& d : ctx.orderedDirs) {
            std::multimap<std::string, py::object> _condsMap;
            for (const auto& c : d.classes) {
                py::object main_module = py::import("__main__");

                py::object main_namespace = main_module.attr("__dict__");

                main_namespace["update"](py::import("re").attr("__dict__"));

                py::object _funcObj = py::eval(("lambda relName: " + c.cond).c_str(), main_namespace);
                _condsMap.emplace(c.class_, _funcObj);
            }
            filterDir(d.path, "", _condsMap, outClasses);
        }
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