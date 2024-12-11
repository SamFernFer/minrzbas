namespace Enums {
    enum class [[clang::annotate("My(0)")]] MyEnum;
    enum class [[clang::annotate("My(1)")]] MyEnum {
        Val1,
        Val2,
        Val3
    };
    enum class [[clang::annotate("Other(0)")]] OtherEnum {
        Val1 = -90,
        Val2 = 0,
        Val3 = -29383423,
        Val4 = 1234
    };
    enum class [[clang::annotate("Other(1)")]] OtherEnum;
    enum class [[clang::annotate("Undefined(0)")]] UndefinedEnum;

    enum class [[clang::annotate("Direct(0)")]] DirectEnum : long long {
        Value1 = 0xFF,
        Value2 = 8,
        Value3 = 7,
        Value4 = 9
    };
    enum [[clang::annotate("Unscoped(0)")]] Unscoped;
    enum [[clang::annotate("Unscoped(1)")]] Unscoped {
        A, Bad, Idea
    };
    enum [[clang::annotate("Unscoped(2)")]] Unscoped;

    enum [[clang::annotate("UnscopedUndefined(0)")]] UnscopedUndefined;
}