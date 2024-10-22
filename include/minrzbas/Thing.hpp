#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-cpp-macro"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
#include <boost/program_options.hpp>
#include <boost/python.hpp>
#pragma clang diagnostic pop
#include <string>
#include <map>
#include <list>

namespace Fenton::Minrzbas {
    // The class is in a struct separate from the directory, because a directory might 
    // have multiple classes, each with their own conditions.
    struct Class {
        // The class the files passing will be added to.
        std::string class_;
        // The Python expression to be used as a condition.
        std::string cond;
    };
    struct Directory {
        // The directory's path. It is recommended that it has no directory separator at the end
        // and that it uses the slash ('/') as the directory separator.
        std::string path;
        // The classes to find files for inside this directory and subdirectories.
        std::list<Class> classes;
    };
    // Simply the content of a script file.
    struct Script {
        std::string content;
    };
    // The processed command-line arguments for use when generating code.
    struct Context {
        // Each class has a map of every event to its associated script.
        std::unordered_map<std::string, std::unordered_map<std::string, Script>> scripts;
        // Each directory has a list of every class which should be checked for in its child files.
        std::unordered_map<std::string, std::list<Class>> dirs;
    };
    Context currentCtx;
    // Adds a directory to the context object.
    void addDirectory(
        Context& ctx,
        std::string_view class_,
        std::string_view path,
        std::string_view cond
    );
    Context getContext(const boost::program_options::variables_map& vm);
    void doThing(const boost::program_options::variables_map& vm) {
        currentCtx = getContext(vm);
    }
}