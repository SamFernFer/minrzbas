[[clang::annotate("Global(0)")]] char c = 90;
[[clang::annotate("Global(0.0)")]] extern const char* str;

[[clang::annotate("Global(0.1)")]] const char* str = "On";

namespace NS {
    class VarType {
    };

    [[clang::annotate("NS(0)")]]
    [[clang::annotate("NS(0.0)")]]
    long** doublePtr;

    class Class {
        VarType privateVar;
    public:
        VarType publicVar;
    };

    struct Struct {
        const VarType* publicVar;
    protected:
        long volatile protectedVar;
    };
}