#ifndef FENTON_NONEMPTY_HPP
#define FENTON_NONEMPTY_HPP

namespace MyUtils {
    constexpr char myChar = 'h';
    template<typename T> struct Property {
        T& parent;
    };
}

namespace MyThings {
    struct MyClass {
    };
}
#endif