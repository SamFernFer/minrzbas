#ifndef FENTON_ALL_HPP
#define FENTON_ALL_HPP

#include <string>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <limits>

// [[clang::annotate("minr::ignored")]];

// namespace Attrs {
//     class Attribute {
//     };
//     class Entity {
//     };
//     class OtherKind {
//     };
//     class [[clang::annotate(R"(minr::attrdecl("custom_thing"))")]] CustomAttr
//      : public Attribute, private Entity, virtual public OtherKind {
//         CustomAttr();
//     };
// }
// class PrinterWrapper {
//     void* obj = nullptr;
//     void (*printFunc)(void* obj) = nullptr;
//     void Print() const {
//         printFunc(obj);
//     }
//     template<typename T> void Init(T&) {
//         obj = const_cast<std::remove_cv_t<T>*>(std::addressof(v));
//         printFunc = [](void* obj)->void {
//             static_cast<T*>(obj)->Print();
//         };
//     }
//     template<typename T> PrinterWrapper(T& v) {
//         Init(v);
//     }
//     template<typename T> PrinterWrapper(T&& v) {
//         Init(v);
//     }
// };
// void doPrint(PrinterWrapper v) {
//     v.Print();
// }

// std::int64_t var = -9808;
// namespace Test1 {
//     #pragma clang attribute push \
//         ([[clang::annotate("minrattr::custom_thing")]],\
//         apply_to = any(function, record))
//     constexpr int val = 5;
//     struct Empty;
//     class
//     // [[clang::annotate("Printable")]]
//     [[
//         clang::annotate("minrattr::printable"),
//         clang::annotate(R"attr(Initialisable, Other, Thing(R"(inner)"))attr"),
//         clang::annotate("Movable")
//     ]]
//     // [[clang::annotate("Movable")]]
//     // [[clang::annotate("Movable")]]
//     // [[clang::annotate("Initialisable")]]
//     Printer {
//         static std::string defaultText = "default";
//         static std::int8_t defaultChar = 7i8;
//         std::string text = "ERROR!";
//     public:
//         Printer();
//         Printer(const std::string& text);
//         [[clang::annotate("Initialisable")]]
//         void SetText(const std::string& text);
//         void SetText(const std::string& text, std::int8_t char_);
//         std::string GetText() const;
//         void Print() const;
//         // Empty statement.
//         ;
//         static void doThing() {
//             struct InnerStruct {
//             };
//             auto _ptr = new InnerStruct;
//             ;
//             delete _ptr;
//         }
//         constexpr static const char* staticStr = "Static text.";
//     };
//     #pragma clang attribute pop
//     static void doOtherThing();
//     struct BaseClass {
//     };
//     struct Other : [[minrattr::custom_attr]] BaseClass {
//         bool bool1 = false;
//         std::string name = "Harold";
//         int i = -10;
//     };
// }
// namespace Test1 {
//     struct Undefined;
//     struct Defined;
//     struct Defined {
//         int _thing = 0x99;
//     };
//     struct Empty {};
//     bool thing() {
//         return false;
//     }
//     struct Undefined;
//     struct Empty;
//     namespace Inner {
//         constexpr const char* _str = "Another";
//         char c = '^';
//     }
//     using TheThing [[minr::ignored]] = Printer;
//     using Other = ::Test1::Other;
    
// }
// namespace Test1 {
//     struct Undefined;
// }
// namespace Aliases {
//     using T = Test1::Printer;
// }
// namespace Aliases::Internal {
//     T thing;
//     T* otherThing = nullptr;
//     nonex nonexVar = {};
// }
// namespace Aliases::Internal {
//     std::int64_t internalVar = 999;
// }
// namespace Aliases::Internal::Even {
//     std::string otherNames = "John and Elise.";
// }
// namespace NS::NS2 {
//     struct ClassThing {
//         long l;
//         int i;
//     }* varPtr;
// }
namespace Enums {
    enum class MyEnum;
    enum class MyEnum {
        Val1,
        Val2,
        Val3
    };
    enum class OtherEnum {
        Val1 = -90,
        Val2 = 0,
        Val3 = -29383423,
        Val4 = 1234
    };
    enum class OtherEnum;
    enum class UdeclaredEnum;

    enum class DirectEnum : std::uint64_t {
        Value1 = std::numeric_limits<std::uint64_t>::max(),
        Value2 = 8,
        Value3 = 7,
        Value4 = 9
    };
    enum Unscoped {
        A, Bad, Idea
    };
}
struct {
    int i, j;
} anoVar;

namespace AnyOther {
    struct {
        int a, b, *c;
    };
}

// namespace Unions {
//     union MyUnion;
//     union MyUnion {
//         Test1::Printer printer;
//         long myLong;
//     };
// }
// union MyUnion {
//     int intVal;
//     long longVal;
//     char charVal;
// };
// union MyUnion;
// union SimpleUnion {};

// struct RandomThing {
//     int i, *j;
// };
// struct RandomThing;

enum class {
    Should, Not, Appear
};

enum class {
    Should, Not, Appear
} howShouldIt;

#endif