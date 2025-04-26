#pragma once
#include <vector>

#include <cmath>

class particle_simulator {
public: // May make it private and add getters and setters later (OOP)
    unsigned short width;
    unsigned short height;
    unsigned short cell_size;
    unsigned short cell_count_x;
    unsigned short cell_count_y;
    double* delta;

    float interaction_radius{};
    size_t particle_count{};
    std::vector<float> positions_x;
    std::vector<float> positions_y;
    std::vector<float> velocities_x;
    std::vector<float> velocities_y;
    std::vector<unsigned char> types;
    std::vector<std::vector<std::vector<size_t>>> particle_grid;

    bool is_space_wrapping_enabled = true;
    bool uses_particle_grid = false;
    float terminal_velocity_strength = 0.f;
    size_t attraction_formula_id = 0;

    void generate_grid();
    // Particle velocity
    void update_particle_velocity(size_t p1, size_t p2, int shift_x, int shift_y);
    void update_particle_position(size_t p);
    // Particle out of bounds behavior
    void wrap_around(size_t p);
    void clamp(size_t p);
    // Particle slow down
    void apply_terminal_velocity(size_t p);
    // Pythagoros
    float calculate_distance(float x1, float y1, float x2, float y2);

public:
    particle_simulator(unsigned short width, unsigned short height, unsigned short cell_size, size_t particle_count, double* delta)
        : width(width), height(height), particle_count(particle_count), delta(delta) {
        resize_cells(cell_size);
    }

    void pre_process();
    void process();

    std::pair<size_t, size_t> convert_coords_to_cell(float x, float y);
    bool does_cell_exist(size_t x, size_t y);
    std::vector<size_t>& get_particles_in_cell(size_t x, size_t y);

    void spawn_particle(float x, float y, unsigned char t);
    void spawn_particle(float x, float y);
    void delete_particle(size_t id);
    void change_particle_count(size_t n); // Chooses random particles to delete/spawn

    void resize_cells(unsigned short size);
    // Getters, setters

};
