#include <utils/Misc.hpp>

#include <iostream>
#include <cctype>
#include <locale>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <memory>

using namespace std::string_literals;
namespace Fenton {
    std::ostream* out = std::addressof(std::cout);

    void setDefaultOutput(std::ostream& newOut) {
        out = &newOut;
    }
    std::ostream& getDefaultOutput() {
        return *out;
    }

    std::string to_string(bool v) {
        return v? "true" : "false";
    }
    void println() {
        *Fenton::out << std::endl;
    }
    void println(std::ostream& stream) {
        stream << std::endl;
    }
    bool is_space(char c) {
        return std::isspace(static_cast<unsigned char>(c));
    }
    bool is_space(char c, const std::locale& loc) {
        return std::isspace(static_cast<unsigned char>(c), loc);
    }
    bool is_control(char c) {
        return std::iscntrl(static_cast<unsigned char>(c));
    }
    bool is_control(char c, const std::locale& loc) {
        return std::iscntrl(static_cast<unsigned char>(c), loc);
    }
    std::string trim(std::string_view str) {
        std::string_view::const_iterator _start, _end;
        for (auto it = str.begin(); it != str.end(); it++) {
            if (!is_space(*it)) {
                _start = it;
                break;
            }
        }
        for (auto it = str.rbegin(); it != str.rend(); it++) {
            if (!is_space(*it)) {
                _end = it.base();
                break;
            }
        }
        return std::string(_start, _end);
    }
    std::string trim_left(std::string_view str) {
        for (auto it = str.begin(); it != str.end(); it++) {
            if (!is_space(*it)) return std::string(it, str.end());
        }
        return {};
    }
    std::string trim_right(std::string_view str) {
        for (auto it = str.rbegin(); it != str.rend(); it++) {
            if (!is_space(*it)) return std::string(str.begin(), it.base());
        }
        return {};
    }
    static char should_escape(char c, bool& isControl) {
        switch (c) {
        case '\\': return '\\';
        case '"': return '\"';
        case '\t': return 't';
        case '\n': return 'n';
        case '\v': return 'v';
        case '\f': return 'f';
        case '\r': return 'r';
        case '\b': return 'b';
        case '\0': return '0';
        default:
            isControl = is_control(c, std::locale::classic());
            return 0;
        }
    }
    std::string escape(std::string_view str) {
        std::stringstream _ss;
        _ss << std::hex
            << std::uppercase
            << std::setfill('0')
        ;
        for (auto it = str.begin(); it != str.end(); it++) {
            bool _isControl = false;
            const char _c = should_escape(*it, _isControl);
            if (_c) {
                _ss.put('\\');
                _ss.put(_c);
            } else if (_isControl) {
                _ss << "\\x"
                    << std::setw(2)
                    << static_cast<unsigned int>(*it)
                    << std::setw(0)
                ;
            } else {
                _ss.put(*it);
            }
        }
        return _ss.str();
    }
    std::string quote(std::string_view str) {
        return "\"" + escape(str) + "\"";
    }
    std::string map_format(std::string_view fmt, const std::map<std::string, std::string>& vars) {
        constexpr auto throwError = [](std::string_view errorText, std::size_t pos)->void {
            throw std::runtime_error(std::vformat(
                "{0} Pos: {1}."s, std::make_format_args(errorText, pos)
            ));
        };
        std::stringstream _ss;
        bool _signFound = false;
        auto it = fmt.begin();
        std::size_t i = 0;
        while (it != fmt.end()) {
            switch (*it) {
            case '$':
                _signFound = !_signFound;
                if (!_signFound) _ss.put(*it);
                break;
            case '{':
                if (_signFound) {
                    it++;
                    bool _hasEnded = false;
                    auto _start = it, _end = it;
                    while (it != fmt.end()) {
                        if (*it == '}') {
                            _hasEnded = true;
                            _end = it;
                            break;
                        }
                        it++;
                        i++;
                    }
                    if (!_hasEnded)
                        throwError("Unclosed variable expansion.", i);
                    if (_start == _end)
                        throwError("Zero-length variable names are not allowed.", i);

                    std::string _varName = std::string(_start, _end);
                    auto _varIt = vars.find(_varName);
                    if (_varIt == vars.end())
                        throwError(std::format("Unknown variable \"{0}\".", _varName), i);
                    _ss << _varIt->second;
                    _signFound = false;
                    break;   
                }
            default:
                if (_signFound) {
                    throwError("An unescaped '$' character must be followed by '{{'", i);
                }
                _ss.put(*it);
                break;
            }
            it++;
            i++;
        }
        return _ss.str();
    }
}