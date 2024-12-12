[[clang::annotate("Global(0)")]] char c = 90;
[[clang::annotate("Global(0.0)")]] extern const char* str;

[[clang::annotate("Global(0.1)")]] extern const char* str;

[[clang::annotate("Global(0.2)")]] const char* str = "On";

namespace NS {
    class VarType {};

    [[clang::annotate("NS(0.0)")]]
    [[clang::annotate("NS(0.1)")]]
    long** doublePtr;

    class Class {
        [[
            clang::annotate("Class(0.0)"),
            clang::annotate("Class(0.1)")
        ]] static VarType privateVar;
    public:
        [[clang::annotate("Class(1)")]] static VarType publicVar;
    };

    struct Struct {
        [[clang::annotate("Class(0)")]] static const VarType* publicVar;
    protected:
        [[clang::annotate("Class(1.0)")]]
        [[clang::annotate("Class(1.1)")]]
        static long volatile protectedVar;
    };
}