char c = 90;
extern const char* str;
const char* str = "On";
namespace NS {
    class VarType {};

    long** doublePtr;

    class Class {
        static VarType privateVar;
    public:
        static VarType publicVar;
    };

    struct Struct {
        static const VarType* publicVar;
    protected:
        static long volatile protectedVar;
    };
}