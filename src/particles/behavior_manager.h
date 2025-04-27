#pragma once
#include <cmath>

class behavior_manager {
public:

    /**
     *
     * @param delta Delta time
     * @param intensity Value 0..1 that determines how fast velocity dampens
     * @return Returns multiplier that has to be multiplied with velocity
     */
    static float calculate_terminal_velocity_change(double delta, float intensity) {
        return std::pow(1-intensity, delta*2.f);
    }

    // Attraction formulas
    static float calculate_attraction_life(float distance, float param_1 = 1) {
        float r = 20; // If particles are too close, then they repel slightly
        float m = 50; // How far does the attraction persist
        if (distance < r) {
            return (distance*distance-r*r)/r/5;
        } else if (distance < m) {
            float g = distance-r;
            return ( -g*g + g*(m-r) )/( m-r )*param_1/5;
        } else {
            return 0.f;
        }
    }

    static float calculate_attraction_newton(float distance, float param_1 = 1) {
        return 1 / distance / distance * 10000.f * param_1;
    }

    static float calculate_attraction_inv_newton(float distance, float param_1 = 1) {
        return distance * distance * 0.001f * param_1;
    }
};