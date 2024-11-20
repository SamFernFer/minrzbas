#include <minrzbas/Thing.hpp>
#include <minrzbas/Program.hpp>
#include <utils/Misc.hpp>

#include <iostream>
#include <utility>
#include <filesystem>

namespace fs = std::filesystem;
namespace po = boost::program_options;

namespace Fenton::Minrzbas {
    void thing(const boost::program_options::variables_map& vm) {
        using namespace Fenton::Minrzbas;
        using namespace std::string_literals;

        std::string _input = vm["input"].as<std::string>();

        std::ostream& _out = std::cout;
        if (vm.count("output")) {
            std::string _output = vm["output"].as<std::string>();
            std::cout << _output << std::endl;
        }

        if (vm.count("input")) {
            
            std::cout << _input << std::endl;
        } else {
            std::cout << 
        }

        const json::object& _in = _vPtr->as_object();
        std::vector<std::string> _args;
        for (const json::value& v : _in.at("includeDirs").as_array()) {
            fs::path _path = v.as_string().c_str();
            if (_path.is_relative())
                _path = pathInRes("testProject1" / _path);
            _args.emplace_back("-I" + _path.string());
        }
        std::string _fileName = pathInRes(_in.at("file").as_string().c_str()).string();
        json::object _actual = unitToJSON(_fileName, _args);
    }
}