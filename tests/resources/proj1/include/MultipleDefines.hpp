// Tests multiple macro definitions from the command-line.

struct Declared1;

// This part should not be compiled.
#ifndef FENTON_UNDECLARED1

struct Undeclared1;

#endif

// This part should also not be compiled.
#ifndef FENTON_UNDECLARED2

struct Undeclared2;

#endif

struct Declared2;