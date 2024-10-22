#pragma once
#ifndef FENTON_FILEUTILS_HPP
#define FENTON_FILEUTILS_HPP
#include <cstdint>
#include <string>

namespace Fenton::FileUtils
{
    // Returns the path to the enclosing executable.
    std::string getExecutablePath();
    // Returns the path to the enclosing module.
    std::string getModulePath();
};
#endif