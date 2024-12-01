struct MyAttr {
    MyAttr(int) {}
};
namespace Attrs {
    enum class MyEnum {
        First, Second, Third
    };
    struct OtherAttr {
        OtherAttr(MyEnum) {}
    };
}