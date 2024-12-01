#include <Attributes.hpp>

struct Upper {
    using Attrs::OtherAttr;
    class [[
        clang::annotate("MyAttr(1)"),
        clang::annotate("MyAttr(2)"),
        clang::annotate("MyAttr(3)")
    ]] NestedPublic {
        class InnerPrivate {
        };
    };
private:
    static constexpr auto fenton_minr_attr_3 = OtherAttr("Thing");
    struct [[clang::annotate(R"(OtherAttr(MyEnum::First))")]] Private {
    };
protected:
    class [[clang::annotate("Thing(0,1)")]] Protected {
    };
};