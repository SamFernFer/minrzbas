#include <string>
#include <cstdint>

std::int64_t var = -9808;
namespace Test1 {
    class [[minr::attrdecl("Printable")]] Printer {
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