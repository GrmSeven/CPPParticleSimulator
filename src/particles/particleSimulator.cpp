#include "particleSimulator.h"

#include <cmath>
#include <iostream>
#include "../utils.h"
#include "behaviorManager.h"

void ParticleSimulator::update_particle_velocity(size_t p1, size_t p2, int shift_x, int shift_y) {
    float distance{};
    float normal_x{};
    float normal_y{};
    float new_pos2_x = positions_x[p2] + shift_x * width;
    float new_pos2_y = positions_y[p2] + shift_y * height;
    distance = calculate_distance(positions_x[p1], positions_y[p1], new_pos2_x, new_pos2_y);
    if (distance != 0) {
        normal_x = (new_pos2_x - positions_x[p1]) / distance;
        normal_y = (new_pos2_y - positions_y[p1]) / distance;
    }
    float force = behaviorManager::calculate_attraction_life(distance, behavior_manager.temp_attraction_table[types[p1] - 'a'][types[p2] - 'a']);
    velocities_x[p1] += force * normal_x;
    velocities_y[p1] += force * normal_y;
}

void ParticleSimulator::update_particle_position(size_t p) {
    positions_x[p] += velocities_x[p] * *delta;
    positions_y[p] += velocities_y[p] * *delta;
}

void ParticleSimulator::generate_grid() {
    // Clears and resizes current particle_grid
    particle_grid.resize(cell_count_y);
    for (auto& y_vec : particle_grid) {
        y_vec.resize(cell_count_x);
        for (auto& x_vec : y_vec) {
            x_vec = std::vector<size_t>();
        }
    }

    // Adds particles into the grid
    std::pair<size_t, size_t> cell;
    for (size_t p_id = 0; p_id < particle_count; p_id++) {
        cell = convert_coords_to_cell(positions_x[p_id], positions_y[p_id]);
        // cout << positions_x[p_id] << ":" << positions_y[p_id] << " " << cell.first << " " << cell.second << endl;
        particle_grid[cell.second][cell.first].push_back(p_id);
    }

}

void ParticleSimulator::wrap_around(size_t p) {
    positions_x[p] = utils::abs_mod(positions_x[p], width); //we dont need to use abs because we havent a negativ vars
    positions_y[p] = utils::abs_mod(positions_y[p], height);;
}

void ParticleSimulator::clamp(size_t p) {
    if (utils::abs_mod(positions_x[p],2.f*width) > width) velocities_x[p] = -velocities_x[p];
    if (utils::abs_mod(positions_y[p],2.f*height) > height) velocities_y[p] = -velocities_y[p];
    positions_x[p] = abs(utils::abs_mod(positions_x[p] + width, 2.f*width)-width);
    positions_y[p] = abs(utils::abs_mod(positions_y[p] + height, 2.f*height)-height);
}

float ParticleSimulator::calculate_distance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


void ParticleSimulator::apply_terminal_velocity(size_t p) {
    float velocity_multiplier = behaviorManager::calculate_terminal_velocity_change(*delta, 0.9f);
    velocities_x[p] *= velocity_multiplier;
    velocities_y[p] *= velocity_multiplier;
}


void ParticleSimulator::pre_process() {
    for (size_t i = 0; i < particle_count; i++) {
        positions_x.push_back(rand() % width);
        positions_y.push_back(rand() % height);
        velocities_x.push_back(0);
        velocities_y.push_back(0);
        types.push_back(char(rand()%2) + 'a'); // Later change to spawn random type
    }
}

void ParticleSimulator::process() {
    if (uses_particle_grid) {
        generate_grid();
        for (size_t p_id = 0; p_id < particle_count; p_id++) { // Hash the distance later
            std::pair<size_t, size_t> cell = convert_coords_to_cell(positions_x[p_id], positions_y[p_id]);

            int cell_radius = ceil(interaction_radius/cell_size);
            int cell_margin_l = -cell_radius + cell.first;
            int cell_margin_r = cell_radius + cell.first;
            int cell_margin_u = -cell_radius + cell.second;
            int cell_margin_d = cell_radius + cell.second;
            cell_margin_l -= (cell_margin_l < 0) ? 1 : 0;
            cell_margin_r += (cell_margin_r > cell_count_x) ? 1 : 0;
            cell_margin_u -= (cell_margin_u < 0) ? 1 : 0;
            cell_margin_d += (cell_margin_d > cell_count_y) ? 1 : 0;
            for (int x = cell_margin_l; x <= cell_margin_r; x++) {
                for (int y = cell_margin_u; y <= cell_margin_d; y++) {
                    for (auto& p2_id : get_particles_in_cell(x, y)) {
                        int shift_x = floor(static_cast<float>(x)/cell_count_x);
                        int shift_y = floor(static_cast<float>(y)/cell_count_y);
                        update_particle_velocity(p_id, p2_id, shift_x, shift_y);
                    }
                }
            }
        }
    } else { // Doesnt work with space wrapping
        for (size_t p_id = 0; p_id < particle_count; p_id++) {
            for (size_t p2_id = 0; p2_id < particle_count; p2_id++) {
                if (p_id != p2_id) {
                    update_particle_velocity(p_id, p2_id, 0, 0);
                }
            }
        }
    }

    for (size_t p_id = 0; p_id < particle_count; p_id++) {
        update_particle_position(p_id);
        apply_terminal_velocity(p_id);
        if (is_space_wrapping_enabled) {
            wrap_around(p_id);
        } else {
            clamp(p_id);
        }
    }
}

std::pair<size_t, size_t> ParticleSimulator::convert_coords_to_cell(float x, float y) {
    return std::make_pair(static_cast<size_t>(floor(x/cell_size)), static_cast<size_t>(floor(y/cell_size)));
}

bool ParticleSimulator::does_cell_exist(size_t x, size_t y) {
    return ceil(width/cell_size) > x && ceil(height/cell_size) > y;
}

std::vector<size_t>& ParticleSimulator::get_particles_in_cell(int x, int y) {
    if (!is_space_wrapping_enabled && !does_cell_exist(x, y)) {
        return empty_vec;
    } else {
        return particle_grid[utils::abs_mod(y, cell_count_y)][utils::abs_mod(x, cell_count_x)];
    }

}

void ParticleSimulator::spawn_particle(float x, float y, unsigned char t) {
    particle_count++;
    positions_x.push_back(x);
    positions_y.push_back(y);
    velocities_x.push_back(0);
    velocities_y.push_back(0);
    types.push_back(t);
}

void ParticleSimulator::spawn_particle(float x, float y) {
    spawn_particle(x, y, 'a');
}

void ParticleSimulator::delete_particle(size_t id) {
    particle_count--;
    positions_x.erase(positions_x.begin() + id);
    positions_y.erase(positions_y.begin() + id);
    velocities_x.erase(velocities_x.begin() + id);
    velocities_y.erase(velocities_y.begin() + id);
    types.erase(types.begin() + id);
}

void ParticleSimulator::change_particle_count(size_t n) {
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
            positions_x[i] = rand() % width;
            positions_y[i] = rand() % height;
            velocities_x[i] = 0;
            velocities_y[i] = 0;
            types[i] = 'a'; // Later change to spawn random type
        }
    }
}

void ParticleSimulator::resize_cells(unsigned short size) {
    cell_size = size;
    cell_count_x = ceil(static_cast<float>(width)/cell_size);
    cell_count_y = ceil(static_cast<float>(height)/cell_size);
}
