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
    enum class UndefinedEnum;

    enum class DirectEnum : long long {
        Value1 = 0xFF,
        Value2 = 8,
        Value3 = 7,
        Value4 = 9
    };
    enum Unscoped;
    enum Unscoped {
        A, Bad, Idea
    };
    enum Unscoped;
    enum UnscopedUndefined;
}