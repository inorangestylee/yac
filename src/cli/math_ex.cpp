#include "math_ex.h"

#include <cmath>

#include "error.h"

unsigned int factorial(unsigned int x) {
    if (x == 0) { return 1; }
    unsigned r = 1;
    for (unsigned int i = 1; i <= x; ++i) {
        r *= i;
    }
    return r;
}

double division(double a, double b, bool is_mod) {
    if (b == 0) {
        Fatal("division by zero");
    }

    double r;

    if (is_mod) {
        r = std::fmod(a, b);
    }
    else {
        r = a / b;
    }

    return r;
}