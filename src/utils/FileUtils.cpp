#include <utils/FileUtils.hpp>

#include <whereami.h>

#include <cstdlib>
#include <stdexcept>

namespace Fenton::FileUtils {
    std::string getExecutablePath() {
        int length = wai_getExecutablePath(NULL, 0, NULL);
        char* path = static_cast<char*>(std::malloc(length + 1));
        wai_getExecutablePath(path, length, NULL);
        path[length] = '\0';
        std::string _str = path;
        std::free(path);
        return _str;
    }
    std::string getModulePath() {
        int length = wai_getModulePath(NULL, 0, NULL);
        char* path = static_cast<char*>(std::malloc(length + 1));
        wai_getExecutablePath(path, length, NULL);
        path[length] = '\0';
        std::string _str = path;
        std::free(path);
        return _str;
    }
}
