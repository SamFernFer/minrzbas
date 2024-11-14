#include <vector>

int sum(int a, int b) {
    return a + b;
}
float sum(float a, float b) {
    return a + b;
}
int sub(int a, int b) {
    return a - b;
}
std::int64_t sub(std::int64_t a, std::int64_t b) {
    return a - b;
}
auto templSum(auto a, auto b) {
    return a + b;
}

namespace ThingNS {
    struct Thing1 {
        std::vector<int> ints;
    };
    struct NonAnonymous { long long_ = 90; };
    Thing1 getThing(Thing1 t1, NonAnonymous t2 = { 90 }) {
        return {};
    }
    Thing1 getThing(Thing1 t1, double d = 0.1) {
        return sizeof(Thing1) * d * t1.ints.size();
    }
}