#pragma once
#include <vector>

#include <cmath>

#include "behaviorManager.h"

class ParticleSimulator {
public: // May make it private and add getters and setters later (OOP)
    bool paused{};
    unsigned short width;
    unsigned short height;
    unsigned short cell_size = 25;
    unsigned short cell_count_x;
    unsigned short cell_count_y;
    double* delta;

    float interaction_radius = 50;
    size_t particle_count = 1000;;
    size_t particle_type_count = 1;
    std::vector<float> positions_x;
    std::vector<float> positions_y;
    std::vector<float> velocities_x;
    std::vector<float> velocities_y;
    std::vector<unsigned short> types;
    std::vector<std::vector<std::vector<size_t>>> particle_grid;
    behaviorManager behavior_manager;

    bool is_space_wrapping_enabled = true;
    bool uses_particle_grid = true;
    bool uses_terminal_velocity = true;
    float terminal_velocity_strength = 0.9f;
    float max_velocity = 1000.f;
    float force_multiplier = 1.f;
    size_t attraction_formula_id = 0;

    std::vector<size_t> empty_vec = {};

    void prepare_grid();
    void generate_grid(size_t p_id);
    // Particle velocity
    void handle_particle_velocity(size_t p_id);
    void update_particle_velocity(size_t p1, size_t p2, int shift_x, int shift_y);
    void update_particle_position(size_t p);
    // Particle out of bounds behavior
    void handle_out_of_bounds(size_t id);
    void wrap_around(size_t p);
    void clamp(size_t p);
    // Particle slow down
    void apply_terminal_velocity(size_t p, float strength);
    // Hypot
    float calculate_distance(float x1, float y1, float x2, float y2);
    // Particle mouse dragging
    void drag_particles(sf::Vector2f from, sf::Vector2f to, float radius, float attraction_force, float drag_curvature);

public:
    ParticleSimulator(unsigned short width, unsigned short height, double* delta)
        : width(width), height(height), delta(delta) {}

    void pre_process();
    void process();

    std::pair<size_t, size_t> convert_coords_to_cell(float x, float y);
    bool does_cell_exist(size_t x, size_t y);
    std::vector<size_t>& get_particles_in_cell(int x, int y);

    void spawn_particle(float x, float y, size_t count, unsigned short t);
    void spawn_particle(float x, float y, size_t count = 1);
    void delete_particle(size_t id);
    void set_particle_count(int n); // Chooses random particles to delete/spawn
    void set_particle_type_count(int n);

    void resize_cells(unsigned short size);
    // Getters, setters

};
