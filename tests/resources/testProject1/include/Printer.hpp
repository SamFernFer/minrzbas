#include <string>
#include <cstdint>

std::int64_t var = -9808;
namespace Test1 {
    struct __attribute__((thing())) Empty;
    class
    [[minr::attrdecl("Printable")]]
    [[clang::annotate("Printable")]]
    __attribute__((a("interesting", 90)))
    Printer {
        std::string text = "ERROR!";
    public:
        Printer();
        Printer(const std::string& text);
        void SetText(const std::string& text);
        std::string GetText() const;
        void Print() const;
        static void doThing();
    };
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