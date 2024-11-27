#include <cstdint>

namespace NS {
    struct OtherStruct {
        bool b;
    private:
        char*const volatile c;
    };
    struct MyStruct {
        OtherStruct os;
    protected:
        OtherStruct*const osPtr;
        long long l;
    private:
        long myLong;
    };
}