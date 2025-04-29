#pragma once
#include <cmath>
#include <vector>

class behaviorManager {
public:
    size_t particle_type_count;
    std::vector<std::vector<float>> temp_attraction_table = {{1, 0}, {0.5, 1}};

    /**
     * @param delta Delta time
     * @param intensity Value 0..1 that determines how fast velocity dampens
     * @return Returns multiplier that has to be multiplied with velocity
     */
    static float calculate_terminal_velocity_change(double delta, float intensity) {
        return std::pow(1-intensity, delta*2.f);
    }

    // Attraction formulas
    static float calculate_attraction_life(float distance, float param) {
        float r = 20; // If particles are too close, then they repel slightly
        float m = 50; // How far does the attraction persist
        if (distance < r) {
            return (distance*distance-r*r)/r/5;
        }
        if (distance < m) {
            float g = distance-r;
            return ( -g*g + g*(m-r) )/( m-r )*param/5;
        }
        return 0.f;
    }

    static float calculate_attraction_linear_life(float distance, float param) {
        float r = 20;
        float m = 50;
        if (distance < r) {
            return (distance - r)/5;
        }
        if (distance < m) {
            return param * ((m-r)/2.f - abs(distance - (m+r)/2.f))/5;
        }
        return 0.f;
    }

    static float calculate_attraction_newton(float distance, float param = 1) {
        return 1 / distance / distance * 10000.f * param;
    }

    static float calculate_attraction_inv_newton(float distance, float param = 1) {
        return distance * distance * 0.001f * param;
    }
};