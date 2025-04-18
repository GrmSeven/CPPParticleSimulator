#pragma once
#include <vector>

#include "SFML/Graphics/Glsl.hpp"
#include "SFML/System/Clock.hpp"

class particle_simulator {
public:
    unsigned int width;
    unsigned int height;
    unsigned int physics_fps;
    particle_simulator(unsigned int width, unsigned int height, unsigned int physics_fps)
        : width(width), height(height), physics_fps(physics_fps) {process();}

    size_t particle_count{};
    std::vector<sf::Glsl::Vec2> positions;
    std::vector<float> velocities;
    std::vector<unsigned char> types;

    sf::Clock clock;
    float delta;

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
