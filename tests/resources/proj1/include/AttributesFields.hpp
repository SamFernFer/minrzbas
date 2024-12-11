#include <cstdint>

namespace NS {
    struct OtherStruct {
        [[clang::annotate("OtherStruct(0)")]] bool b;
    private:
        [[clang::annotate("OtherStruct(1)")]] char*const volatile c;
    };
    struct MyStruct {
        [[
            clang::annotate("MyStruct(0)"),
            clang::annotate("MyStruct(0.0)"),
            clang::annotate("MyStruct(0.1)")
        ]] OtherStruct os;
    protected:
        [[clang::annotate("MyStruct(1)")]]
        [[clang::annotate("MyStruct(1.0)")]]
        [[clang::annotate("MyStruct(1.1)")]] OtherStruct*const osPtr;

        [[clang::annotate("MyStruct(2)")]] long long l;
    private:
        [[clang::annotate("MyStruct(3)")]] long myLong;
    };
}