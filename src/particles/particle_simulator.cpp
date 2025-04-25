#include "particle_simulator.h"
#include "."

#include <cmath>

void particle_simulator::update_position() {
}

void particle_simulator::generate_grid() {
    // Clears and resizes current particle_grid
    particle_grid.resize(ceil(height/cell_size));
    for (auto& y_vec : particle_grid) {
        y_vec.resize(ceil(width/cell_size));
        for (auto& x_vec : y_vec) {
            x_vec = std::vector<size_t>();
        }
    }

    // Adds particles into the grid
    for (size_t p_id = 0; p_id < particle_count; p_id++) {
        std::pair<size_t, size_t> cell = convert_coords_to_cell(positions_x[p_id], positions_y[p_id]);
        particle_grid[cell.first][cell.second].push_back(p_id);
    }
}

void particle_simulator::wrap_around() {
}

void particle_simulator::clamp() {
}

void particle_simulator::terminal_velocity() {
}

void particle_simulator::slow_down_velocity() {
}

void particle_simulator::pre_process() {
    for (size_t i = 0; i < particle_count; i++) {
        positions_x[i] = std::rand() % width;
        positions_y[i] = std::rand() % height;
        velocities_x[i] = 0;
        velocities_y[i] = 0;
        types[i] = 'a'; // Later change to spawn random type
    }
}

void particle_simulator::process() {
    generate_grid();
    for (size_t p_id = 0; p_id < particle_count; p_id++) { // Hash the distance later
        std::pair<size_t, size_t> cell = convert_coords_to_cell(positions_x[p_id], positions_y[p_id]);
        size_t cell_radius = ceil(interaction_radius/cell_size);
        for (size_t x = -cell_radius; x >= cell_radius; x++) {
            for (size_t y = -cell_radius; y >= cell_radius; y++) {
                for (auto& p2_id : get_particles_in_cell(x, y)) {
                    //
                };
            }
        };
    }
}

std::pair<size_t, size_t> particle_simulator::convert_coords_to_cell(float x, float y) {
}

bool particle_simulator::does_cell_exist(size_t x, size_t y) {
}

std::vector<size_t> & particle_simulator::get_particles_in_cell(size_t x, size_t y) {
}

void particle_simulator::spawn_particle(float x, float y, unsigned char t) {
    particle_count++;
    positions_x.push_back(x);
    positions_y.push_back(y);
    velocities_x.push_back(0);
    velocities_y.push_back(0);
    types.push_back(t);
}

void particle_simulator::spawn_particle(float x, float y) {
    spawn_particle(x, y, 'a');
}

void particle_simulator::delete_particle(size_t id) {
    particle_count--;
    positions_x.erase(positions_x.begin() + id);
    positions_y.erase(positions_y.begin() + id);
    velocities_x.erase(velocities_x.begin() + id);
    velocities_y.erase(velocities_y.begin() + id);
    types.erase(types.begin() + id);
}

void particle_simulator::change_particle_count(size_t n) {
    if (n == particle_count) return;
    if (n < particle_count) {
        particle_count = n;
        positions_x.resize(n);
        positions_y.resize(n);
        velocities_x.resize(n);
        velocities_y.resize(n);
        types.resize(n);
    }
    if (n > particle_count) {
        for (size_t i = particle_count; i < n - 1; i++) {
            positions_x[i] = std::rand() % width;
            positions_y[i] = std::rand() % height;
            velocities_x[i] = 0;
            velocities_y[i] = 0;
            types[i] = 'a'; // Later change to spawn random type
        }
    }
}
