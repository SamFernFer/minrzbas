#include <string>
#include <cstdint>

[[clang::annotate("minr::ignored")]];

std::int64_t var = -9808;
namespace Test1 {
    #pragma clang attribute push ([[clang::annotate("minr::ignored")]], apply_to = any(function, record))
    struct Empty;
    class
    // [[clang::annotate("Printable")]]
    [[
        clang::annotate("minrattr::printable"),
        clang::annotate(R"attr(Initialisable, Other, Thing(R"(inner)"))attr"),
        clang::annotate("Movable")
    ]]
    // [[clang::annotate("Movable")]]
    // [[clang::annotate("Movable")]]
    // [[clang::annotate("Initialisable")]]
    Printer {
        std::string text = "ERROR!";
    public:
        Printer();
        Printer(const std::string& text);
        [[clang::annotate("Initialisable")]]
        void SetText(const std::string& text);
        std::string GetText() const;
        void Print() const;
        static void doThing() {
            struct InnerStruct {
            };
            auto _ptr = new InnerStruct;
            delete _ptr;
        }
    };
    #pragma clang attribute pop
    static void doOtherThing();
    struct Other {
        bool bool1 = false;
        std::string name = "Harold";
        int i = -10;
    };
}
namespace Test1 {
    struct Undeclared;
    struct Declared {
        int _thing = 0x99;
    };
    struct Declared;
    struct Empty {};
    bool thing() {
        return false;
    }
    namespace Inner {
        constexpr const char* _str = "Another";
        char c = '^';
    }
    using TheThing = Printer;
    using Other = ::Test1::Other;
}
namespace Aliases {
    using T = Test1::Printer;
}
namespace Aliases::Internal {
    T thing;
    T* otherThing = nullptr;
    nonex nonexVar = {};
}
namespace Aliases::Internal {
    std::int64_t internalVar = 999;
}
namespace Aliases::Internal::Even {
    std::string otherNames = "John and Elise.";
}