char c = 90;
const char* str = "On";
namespace NS {
    class VarType {
    };

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