#ifndef FENTON_FUNCTIONS_HPP
#define FENTON_FUNCTIONS_HPP

int sum(int a, int b) {
    return a + b;
}
float sum(float a, float b) {
    return a + b;
}
int sub(int a, int b) {
    return a - b;
}
long long sub(long long a, long long b) {
    return a - b;
}

namespace ThingNS {
    struct Thing1 {};
    struct NonAnonymous {};

    Thing1 getThing(Thing1 t1, double d = 0.1);

    Thing1 getThing(Thing1 t1, NonAnonymous t2) {
        return {};
    }
    Thing1 getThing(Thing1 t1, double d) {
        return {};
    }
    Thing1 getThing(Thing1 t1, float f = 0.1) {
        return {};
    }
    Thing1 getThing(Thing1 t1, char d = 8);

    Thing1 getThing(Thing1 t1, char d) {
        return {};
    }
    struct FullStruct {
    };
    Thing1 getThing(Thing1 t1, FullStruct fs = FullStruct{}, float f = 9.1f) {
        return {};
    }
    const char* wasteful(const char* str = "Hello World!") {
        return str;
    }

    Thing1 operator+(const Thing1& a, const Thing1& b);
}
float operator/(const ThingNS::Thing1& t, const ThingNS::NonAnonymous& n);

#endif