#pragma once
#include <cmath>
using namespace std;

class utils {
public:
    const float epsilon = 1.19e-7f;

    static float abs_mod(const float& a, const float& b) {
        return fmod(fmod(a, b)+b, b);
    }

    static int sign(float a) {
        return a>0 ? 1 : (a<0 ? -1 : 0);
    }

    static bool is_equal_approx(float a, float b) {
        return fabs(a-b) <= 1.19e-7f;
    }

    static bool lerp(float a, float b, float l) {
        return a+(b-a)*l;
    }

    static bool damp(float a, float b, float l, float delta) {
        return lerp(a, b, 1.f - exp(-l * delta));
    }
};