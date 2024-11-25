#ifndef FENTON_UTILS_MISC_HPP
#define FENTON_UTILS_MISC_HPP

#include <format>
#include <functional>
#include <string>
#include <iostream>
#include <unordered_map>

namespace Fenton {
    extern std::ostream* out;

    // Sets the default output stream for the printing functions.
    void setDefaultOutput(std::ostream& newOut);
    // Returns the current default output stream for the printing functions.
    std::ostream& getDefaultOutput();

    std::string to_string(bool v);
    template<typename...A> std::string ftformat(std::string_view str, A&&... args) {
        return std::vformat(str.c_str(), std::make_format_args(args...));
    }
#ifndef FENTON_UTILS_DISABLE_STDOUT_PRINT
    // Prints the value.
    template<typename T> void print(const T& v) {
        (*Fenton::out) << v;
    }
    // Prints the string, formatted with the following arguments.
    template<typename...A> static void printf(std::string_view str, A&&... args) {
        print(std::vformat(str.c_str(), std::make_format_args(args...)));
    }
    // Prints a new-line.
    void println();
    // Prints the value, followed by a new-line.
    template<typename T> void println(const T& v) {
        (*Fenton::out) << v << std::endl;
    }
    // Prints the string, formatted with the following arguments, followed by a new-line.
    template<typename...A> static void printlnf(std::string_view str, A&&... args) {
        println(std::vformat(str, std::make_format_args(args...)));
    }
#endif

    // Writes the text to the stream.
    template<typename T> void print(std::ostream& stream, const T& v) {
        stream << v;
    }
    // Writes the text to the string, formatted with the following arguments.
    template<typename...A> static void printf(std::ostream& stream, std::string_view str, A&&... args) {
        print(stream, std::vformat(str.c_str(), std::make_format_args(args...)));
    }
    // Writes a new-line to the stream.
    void println(std::ostream& stream);
    // Writes the text to the stream, followed by a new-line.
    template<typename T> void println(std::ostream& stream, const T& v) {
        stream << v << std::endl;
    }
    // Writes the text to the stream, formatted with the following arguments, followed by a new-line.
    template<typename...A> static void printlnf(std::ostream& stream, std::string_view str, A&&... args) {
        println(stream, std::vformat(str.c_str(), std::make_format_args(args...)));
    }
    // Trims the space in the beginning and end.
    std::string trim(std::string_view str);
    // Trims the space in the beginning.
    std::string trim_left(std::string_view str);
    // Trims the space in the end.
    std::string trim_right(std::string_view str);
    // Escapes the characters '"', '\\', '\n', '\r', '\t', '\f' and '\0' with the '\\' character.
    std::string escape(std::string_view str);
    // Return the result of processing the string str with the escape function and 
    // within double quotes.
    std::string quote(std::string_view str);
    // Formats the UTF-8-encoded string fmt so that every occurence of ${var_name} is replace by the 
    // value of the variable named var_name in the map vars. Throws an exception if there is an attempt 
    // to use a variable not present in the variable map. To escape the the '$' character, use "$$". Curly 
    // brackets ("{" and "}") need no escaping if they do not have an unescaped '$' character right before 
    // them.
    std::string map_format(
        std::string_view fmt,
        const std::unordered_map<std::string, std::string>& vars
    );
    // std::string map_format_visit(std::string_view fmt, const std::map<std::string, std::string>& vars, 
    //     std::function<std::string(std::string_view name, std::size_t pos, bool& success)> visitor);
}
#endif