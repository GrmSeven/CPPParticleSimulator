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
    std::vector<std::vector<float>> particle_interaction_matrix = {
        {1, 0, 0, 0, 0, 0, 0, 0.5},
        {0.5, 1, 0, 0, 0, 0, 0, 0},
        {0, 0.5, 1, 0, 0, 0, 0, 0},
        {0, 0, 0.5, 1, 0, 0, 0, 0},
        {0, 0, 0, 0.5, 1, 0, 0, 0},
        {0, 0, 0, 0, 0.5, 1, 0, 0},
        {0, 0, 0, 0, 0, 0.5, 1, 0},
        {0, 0, 0, 0, 0, 0, 0.5, 1}
    };

    float min_distance = 20.f;  // If particles are too close, then they repel slightly
    float interaction_radius = 50.f;  // How far does the attraction persist

    void resize_matrix(size_t s) {
        if (particle_type_count == s) return;
        for (size_t i = 0; i < particle_type_count; i++) {
            particle_interaction_matrix[i].resize(s, 0);
        }
        particle_interaction_matrix.resize(s, std::vector<float>(s, 0));
        particle_type_count = s;
    }

    void randomize_matrix() {
        for (size_t i = 0; i < particle_type_count; i++) {
            for (size_t j = 0; j < particle_type_count; j++) {
                particle_interaction_matrix[i][j] = (rand() % 5)/2.5f - 1;
            }
        }
    }

    float calculate_attraction(size_t attraction_type, float distance, float param) {
        switch (attraction_type) {
            case 0:
                return calculate_attraction_life(distance, param);
            case 1:
                return calculate_attraction_newton(distance, param);
            case 2:
                return calculate_attraction_inv_newton(distance, param);
        }
    }

    sf::Color get_particle_color(size_t type) {
        return get_palette_color(fmod(type/static_cast<float>(particle_type_count), 1.f), {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}, {0.00, 0.67, 0.33});
    }

    sf::Color get_palette_color(float t, sf::Vector3f a, sf::Vector3f b, sf::Vector3f c, sf::Vector3f d) {
        return sf::Color((a.x + b.x*cos( 2*M_PI*(c.x*t + d.x)))*255.f, (a.y + b.y*cos( 2*M_PI*(c.y*t + d.y)))*255.f, (a.z + b.z*cos( 2*M_PI*(c.z*t + d.z)))*255.f);
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
    float calculate_attraction_life(float distance, float param) {
        if (distance < min_distance) {
            return (distance*distance-min_distance*min_distance)/min_distance/5;
        }
        if (distance < interaction_radius) {
            float g = distance-min_distance;
            return ( -g*g + g*(interaction_radius-min_distance) )/( interaction_radius-min_distance )*param/5;
        }
        return 0.f;
    }

    float calculate_attraction_linear_life(float distance, float param) {
        if (distance < min_distance) {
            return (distance - min_distance)/5;
        }
        if (distance < interaction_radius) {
            return param * ((interaction_radius-min_distance)/2.f - abs(distance - (interaction_radius+min_distance)/2.f))/5;
        }
        return 0.f;
    }

    float calculate_attraction_newton(float distance, float param) {
        return 1 / distance / distance * 10000.f * param;
    }

    float calculate_attraction_inv_newton(float distance, float param) {
        return distance * distance * 0.001f * param;
    }
};