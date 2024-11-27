// Tests multiple macro definitions from the command-line.

struct RandomType {
    bool b1, b2, b3;
};

// This part should not be compiled.
#ifndef FENTON_UNDEFINED_FUNC

int undefined_func(const RandomType& rt) {
    return 9;
}

#endif

// This part should also not be compiled.
#ifndef FENTON_OTHER_FUNC

int other_func(const RandomType& rt) {
    return 9;
}

#endif

// This should work already, but the location of "const" is different here.
int defined_func(RandomType const& rt) {
    return 0;
}