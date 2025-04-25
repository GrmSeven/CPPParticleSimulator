#include "particle_simulator.h"

#include <cmath>
#include "../utils.h"

void particle_simulator::update_particle_velocity(size_t p1, size_t p2) {
}

void particle_simulator::update_particle_position(size_t p) {
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

void particle_simulator::wrap_around(size_t p) {
    positions_x[p] = utils::abs_mod(positions_x[p], width); //we dont need to use abs because we havent a negativ vars
    positions_y[p] = utils::abs_mod(positions_y[p], height);;
}

void particle_simulator::clamp(size_t p) {
    if (utils::abs_mod(positions_x[p],2.f*width) > width) velocities_x[p] = -velocities_x[p];
    if (utils::abs_mod(positions_y[p],2.f*height) > height) velocities_y[p] = -velocities_y[p];
    positions_x[p] = abs(utils::abs_mod(positions_x[p] + width, 2.f*width)-width);
    positions_y[p] = abs(utils::abs_mod(positions_y[p] + height, 2.f*height)-height);
}


void particle_simulator::apply_terminal_velocity(size_t p) {
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
    if (uses_particle_grid) {
        generate_grid();
        for (size_t p_id = 0; p_id < particle_count; p_id++) { // Hash the distance later
            std::pair<size_t, size_t> cell = convert_coords_to_cell(positions_x[p_id], positions_y[p_id]);
            size_t cell_radius = ceil(interaction_radius/cell_size);
            for (size_t x = -cell_radius + cell.first; x >= cell_radius + cell.first; x++) { // Should also account for when cell is cut in half with wrapping
                for (size_t y = -cell_radius + cell.second; y >= cell_radius + cell.second; y++) {
                    for (auto& p2_id : get_particles_in_cell(x, y)) {
                        update_particle_velocity(p_id, p2_id);
                    };
                }
            };
        }
    } else {
        for (size_t p_id = 0; p_id < particle_count - 1; p_id++) {
            for (size_t p2_id = p_id + 1; p2_id < particle_count; p2_id++) {
                update_particle_velocity(p_id, p2_id);
            }
        }
    }

    for (size_t p_id = 0; p_id < particle_count; p_id++) {
        apply_terminal_velocity(p_id);
        update_particle_position(particle_count);
        if (is_space_wrapping_enabled) {
            wrap_around(p_id);
        } else {
            clamp(p_id);
        }
    }
}

std::pair<size_t, size_t> particle_simulator::convert_coords_to_cell(float x, float y) {
    return std::make_pair(static_cast<size_t>(floor(x/cell_size)), static_cast<size_t>(floor(y/cell_size)));
}

bool particle_simulator::does_cell_exist(size_t x, size_t y) {
    return ceil(width/cell_size) > x && ceil(height/cell_size) > y;
}

std::vector<size_t>& particle_simulator::get_particles_in_cell(size_t x, size_t y) {
    if (does_cell_exist(x, y)) {
        return particle_grid[x][y];
    } else {
        if (is_space_wrapping_enabled) {
            return particle_grid[utils::abs_mod(x, 2)][y];
        } else {
            std::vector<size_t> empty_vec = std::vector<size_t>();
            return empty_vec;
        }
    }
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

void particle_simulator::resize_cells(unsigned short size) {
    cell_size = size;
    cell_count_x = ceil(width/cell_size);
    cell_count_y = ceil(height/cell_size);
}
