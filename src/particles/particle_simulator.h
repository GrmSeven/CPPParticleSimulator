#pragma once
#include <vector>

#include "SFML/Graphics/Glsl.hpp"

class particle_simulator {
public: // May make it private and add getters and setters later (OOP)
    unsigned short width;
    unsigned short height;
    unsigned short cell_size;
    double* delta;

    float interaction_radius{};
    size_t particle_count{};
    std::vector<float> positions_x;
    std::vector<float> positions_y;
    std::vector<float> velocities_x;
    std::vector<float> velocities_y;
    std::vector<unsigned char> types;
    std::vector<std::vector<std::vector<size_t>>> particle_grid;

    enum SpaceTypes {
        WRAPPING,
        CLAMPED,
        INFINITE
    };
    SpaceTypes space_type = WRAPPING;
    bool uses_particle_grid = true;
    float terminal_velocity_strength = 0.f;
    size_t attraction_formula_id = 0;

    // Particle velocity
    void update_position();
    void generate_grid();
    // Particle out of bounds behavior
    void wrap_around();
    void clamp();
    // Particle slow down
    void terminal_velocity();
    void slow_down_velocity();

public:
    particle_simulator(unsigned short width, unsigned short height, unsigned short cell_size, size_t particle_count, double* delta)
        : width(width), height(height), cell_size(cell_size), particle_count(particle_count), delta(delta) {}

    void pre_process();
    void process();

    std::pair<size_t, size_t> convert_coords_to_cell(float x, float y);
    bool does_cell_exist(size_t x, size_t y);
    std::vector<size_t>& get_particles_in_cell(size_t x, size_t y);

    void spawn_particle(float x, float y, unsigned char t);
    void spawn_particle(float x, float y);
    void delete_particle(size_t id);
    void change_particle_count(size_t n); // Chooses random particles to delete/spawn

    // Getters, setters

};
