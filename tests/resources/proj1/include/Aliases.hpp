namespace Original::Inner {
    struct Simple {};
}

namespace Aliases {
    using S = Original::Inner::Simple;
    typedef Original::Inner::Simple T;

    using TT = T;
    typedef S SS;

    using PtrT = long long* const *;

    using PtrTT = PtrT;

    class Class {
        using C = Class*;
        using CStr = const C*;
    public:
        using P = PtrTT;
    protected:
        typedef TT* PtrT;
    };
}