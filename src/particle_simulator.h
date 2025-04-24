#pragma once
#include <vector>

#include "SFML/Graphics/Glsl.hpp"
#include "SFML/System/Clock.hpp"

class particle_simulator {
public:
    unsigned int width;
    unsigned int height;
    unsigned short cell_size;
    double delta;
    particle_simulator(unsigned short width, unsigned short height, unsigned short cell_size, double delta)
        : width(width), height(height), cell_size(cell_size), delta(delta) {process();}

    size_t particle_count{};
    std::vector<sf::Vector2<float>> positions;
    std::vector<sf::Vector2<float>> velocities;
    std::vector<unsigned char> types;

    void pre_process();
    void process();

    // Particle velocity
    void update();
    // Particle out of bounds behavior
    void clamp();
    void reflect();
    // Particle slow down
    void terminal_velocity();
    void slow_down_velocity();

private:
};
