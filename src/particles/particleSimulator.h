#pragma once
#include <vector>
#include <cmath>
#include "behaviorManager.h"
#include "../userInterface/userInterface.h"

class ParticleSimulator {
public: // May make it private and add getters and setters later (OOP)
    bool paused{};
    bool use_multithreading = true;
    bool use_particle_grid = true;
    unsigned short width;
    unsigned short height;
    float cell_size;
    unsigned short cell_count_x;
    unsigned short cell_count_y;
    float* delta;

    UserInterface* user_interface;

    size_t particle_count = 1000;;
    std::vector<float> positions_x;
    std::vector<float> positions_y;
    std::vector<float> velocities_x;
    std::vector<float> velocities_y;
    std::vector<unsigned short> types;
    std::vector<std::vector<std::vector<size_t>>> particle_grid;
    behaviorManager behavior_manager;

    bool is_space_wrapping_enabled = true;

    bool uses_terminal_velocity = true;
    float terminal_velocity_strength = 0.9f;
    float max_velocity = 1000.f;
    float force_multiplier = 2.f;
    size_t attraction_formula_id = 0;

    std::vector<size_t> empty_vec = {};

    void prepare_grid();
    void generate_grid(size_t p_id);
    // Particle velocity
    void handle_particle_velocity(size_t p_id);
    void update_particle_velocity(size_t p1, size_t p2);
    void update_particle_position(size_t p);
    // Particle out of bounds behavior
    void handle_out_of_bounds(size_t id);
    void wrap_around(size_t p);
    void clamp(size_t p);
    // Particle slow down
    void apply_terminal_velocity(size_t p, float strength);
    // Distance
    float fast_inv_sqrt(float x);
    // Particle mouse dragging
    void drag_particles(sf::Vector2f from, sf::Vector2f to, float radius, float attraction_force, float drag_curvature, bool drag_type);

public:
    ParticleSimulator(unsigned short width, unsigned short height, float* delta, UserInterface* user_interface)
        : width(width), height(height), delta(delta), user_interface(user_interface) {
        pre_process();
    }

    void pre_process();
    void process();

    std::pair<int, int> convert_coords_to_cell(float x, float y);
    bool does_cell_exist(size_t x, size_t y);
    std::vector<size_t>& get_particles_in_cell(int x, int y);

    void sync_settings();
    void spawn_particle(float x, float y, size_t count, unsigned short t);
    void spawn_particle(float x, float y, size_t count = 1);
    void delete_particle(size_t id);
    void delete_particle_near(sf::Vector2f pos, float radius);
    void set_particle_count(int n); // Chooses random particles to delete/spawn
    void set_particle_type_count();

    void resize_cells(float size);
    // Getters, setters

};
