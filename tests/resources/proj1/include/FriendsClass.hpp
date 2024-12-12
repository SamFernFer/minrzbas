class Friend {};
struct Other;

class Me {
    friend class Friend;
    friend class Other;
};

namespace Distant::Horizon {
    struct Friend {};
}
namespace Distant {
    class Other;
}

namespace Scope {
    class Inner {
        friend Distant::Horizon::Friend;
        friend Distant::Other;
    };
}