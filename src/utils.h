#pragma once
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "SFML/System/Vector2.hpp"
using namespace std;

class utils {
public:
    const float epsilon = 1.19e-7f;

    static float abs_mod(const float& a, const float& b) {
        return fmod(fmod(a, b)+b, b);
    }

    static sf::Vector2f abs_mod(const sf::Vector2f& a, const sf::Vector2f& b) {
        return {abs_mod(a.x, b.x), abs_mod(a.y, b.y)};
    }

    static int sign(float a) {
        return a>0 ? 1 : (a<0 ? -1 : 0);
    }

    static bool is_equal_approx(float a, float b) {
        return fabs(a-b) <= 1.19e-7f;
    }

    template <typename T>
    static sf::Vector2<T> lerp(sf::Vector2<T> a, sf::Vector2<T> b, float l) {
        return sf::Vector2(a.x+b.x, a.y+b.y) * l;
    }

    static sf::Color lerp(sf::Color a, sf::Color b, float l) {
        return {lerp(a.r, b.r, l), lerp(a.g, b.g, l), lerp(a.b, b.b, l)};
    }

    template <typename T>
    static T lerp(T a, T b, float l) {
        return a+(b-a)*l;
    }

    static bool damp(float a, float b, float l, float delta) {
        return lerp(a, b, 1.f - exp(-l * delta));
    }

    static float clamp(float value, float min_value, float max_value) {
        return min(max(value, min_value), max_value);
    }


    static void openWebPage(const char* url) {
        #ifdef _WIN32
                std::string command = "start " + std::string(url);
        #elif __APPLE__
                std::string command = "open " + std::string(url);
        #elif __linux__
                std::string command = "xdg-open " + std::string(url);
        #else
                return;
        #endif

        system(command.c_str());
    }
};