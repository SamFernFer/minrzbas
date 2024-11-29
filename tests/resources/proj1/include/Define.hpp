// Tests macro definitions from the command-line.

struct Declared1;

// This part should not be compiled.
#ifndef FENTON_UNDECLARED

struct Undeclared;

#endif

struct Declared2;