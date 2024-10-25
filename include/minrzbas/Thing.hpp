#ifndef FENTON_MINRZBAS_THING_HPP
#define FENTON_MINRZBAS_THING_HPP

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmicrosoft-cpp-macro"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
#include <boost/program_options.hpp>
#pragma clang diagnostic pop
#include <string>
#include <map>
#include <list>

namespace Fenton::Minrzbas {
    constexpr char* _hppDefaultCond = "relName.endswith('.hpp')";
    constexpr char* _cppDefaultCond = "relName.endswith('.cpp')";

    // The class is in a struct separate from the directory, because a directory might 
    // have multiple classes, each with their own conditions.
    struct Class {
        // Each file which passes the condition will be added to this class.
        std::string class_;
        // The Python expression to be used as a condition.
        std::string cond;
        bool operator==(const Class& other) const {
            return this->class_ == other.class_ && this->cond == other.cond;
        }
    };
    struct Directory {
        // The directory's path. It is recommended that it has no directory separator at the end
        // and that it uses the slash ('/') as the directory separator.
        std::string path;
        // The classes to find files for inside this directory and subdirectories.
        std::list<Class> classes;
        bool operator==(const Directory& other) const {
            return this->path == other.path && this->classes == other.classes;
        }
    };
    // struct FilteredClass {
    //     // The class' name.
    //     std::string_view class_;
    //     // The filtered files.
    //     std::list<std::filesystem::path> files;
    //     bool operator==(const FilteredClass& other) const {
    //         return this->class_ == other.class_ && this->files == other.files;
    //     }
    // };
    // struct FilteredDirectory {
    //     // The directory's path.
    //     std::string_view path;
    //     // The classes to find files for inside this directory and subdirectories.
    //     std::list<FilteredClass> classes;
    //     bool operator==(const FilteredDirectory& other) const {
    //         return this->path == other.path && this->classes == other.classes;
    //     }
    // };
    // Simply the content of a script file.
    struct Script {
        std::string content;
    };
    // The processed command-line arguments for use when generating code.
    struct Context {
        std::string srcdir, resdir;
        // Each class has a map of every event to its associated script.
        std::unordered_map<std::string, std::unordered_map<std::string, Script>> scripts;
        // The directories in the order they were specified in.
        std::list<Directory> orderedDirs;
        // Each directory has a list of every class which should be checked for in its child files.
        // The keys are std::string_view so that there's no need for the map to be heterogeneous.
        std::unordered_map<std::string, Directory*> dirs;
    };
    // Adds a directory to the context object.
    void addDirectory(
        Context& ctx,
        std::string_view class_,
        std::string_view path,
        std::string_view cond
    );
    Context getContext(const boost::program_options::variables_map& vm);
    void doThing(const boost::program_options::variables_map& vm);
}
#endif