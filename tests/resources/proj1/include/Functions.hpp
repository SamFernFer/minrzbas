#ifndef FENTON_FUNCTIONS_HPP
#define FENTON_FUNCTIONS_HPP

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
    enum class ThingType {
    };
    struct Thing1 {
        static std::vector<int> ints;
        long myLong = 1<<4;

        static void function() noexcept {
            ints.emplace_back(ints.size());
        }
        static Thing1 New(long myLong);

        long getLong() {
            return myLong;
        }

        Thing1() = default;
        Thing1(long myLong);
        explicit Thing1(ThingType type) noexcept;

        ~Thing1() noexcept;
        ~Thing1(int i);
        ~Thing1(int i) noexcept;
        virtual ~Thing1(Thing1* ptr);
    };
    Thing1 Thing1::New(long myLong) {
        return Thing1(myLong);
    }
    Thing1::Thing1(long myLong) {
        this->myLong = myLong;
    }

    struct NonAnonymous { long long_ = 90; };
    Thing1 getThing(Thing1 t1, NonAnonymous t2 = { 90 }) {
        return {};
    }
    Thing1 getThing(Thing1 t1, double d = 0.1) {
        return sizeof(Thing1) * d * t1.ints.size();
    }
}

#endif