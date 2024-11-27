#include <stdint>

namespace NS {
    struct OtherStruct {
        bool b;
        std::int8_t c;
    };
    using ShortName = OtherStruct;
    struct MyStruct {
        ShortName os;
        std::int64_t l;
        long myLong;
    };
}