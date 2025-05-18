#pragma once
#include <cmath>
#include <vector>
#include <iostream>

#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"

using namespace std;

class behaviorManager {
public:
    size_t particle_type_count = 8;
    std::vector<std::vector<float>> particle_interaction_matrix;

    float min_distance = 20.f;  // If particles are too close, then they repel slightly
    float interaction_radius = 50.f;  // How far does the attraction persist

    float calculate_attraction(const size_t attraction_type, float distance, const float param) {
        switch (attraction_type) {
            case 0:
                return calculate_attraction_in_out(distance, param);
            case 1:
                return calculate_attraction_atom(distance, param);
            case 2:
                return calculate_attraction_in(distance, param);
            case 3:
                return calculate_attraction_out(distance, param);
            case 4:
                return calculate_attraction_const(distance, param);
        }
        return 0;
    }

    /**
     * @param delta Delta time
     * @param intensity Value 0..1 that determines how fast velocity dampens
     * @return Returns multiplier that has to be multiplied with velocity
     */
    float calculate_terminal_velocity_change(double delta, float intensity) {
        return std::pow(1-intensity, delta*2.f);
    }

private:
    // Attraction formulas
    float calculate_attraction_in_out(float distance, float param) {
        if (distance < min_distance) {
            return (distance - min_distance)/5.f;
        }
        if (distance < interaction_radius) {
            return param*((interaction_radius-min_distance)/2 - abs((interaction_radius+min_distance)/2-distance))/5.f;
        }
        return 0.f;
    }

    float calculate_attraction_in(float distance, float param) {
        if (distance < min_distance) {
            return (distance - min_distance)/5.f;
        }
        if (distance < interaction_radius) {
            return param*(distance - min_distance)/5.f;
        }
        return 0.f;
    }

    float calculate_attraction_out(float distance, float param) {
        if (distance < min_distance) {
            return (distance - min_distance)/5.f;
        }
        if (distance < interaction_radius) {
            return param*(interaction_radius - distance)/5.f;
        }
        return 0.f;
    }

    float calculate_attraction_const(float distance, float param) {
        if (distance < min_distance) {
            return (distance - min_distance)/5.f;
        }
        if (distance < interaction_radius) {
            return param*interaction_radius/4.f/5.f;
        }
        return 0.f;
    }

    float calculate_attraction_atom(float distance, float param) {
        if (distance < min_distance) {
            return (distance - min_distance)/5.f;
        }
        if (distance < interaction_radius) {
            return param*(distance - min_distance - interaction_radius/2)/5.f;
        }
        return 0.f;
    }
};