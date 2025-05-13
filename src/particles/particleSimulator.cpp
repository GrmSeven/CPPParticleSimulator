#include "particleSimulator.h"

#include <cmath>
#include <iostream>
#include "../utils.h"
#include "behaviorManager.h"

void ParticleSimulator::pre_process() {
    resize_cells(cell_size);
    for (size_t i = 0; i < particle_count; i++) {
        positions_x.push_back(rand() % width);
        positions_y.push_back(rand() % height);
        velocities_x.push_back(0);
        velocities_y.push_back(0);
        types.push_back(char(rand()%behavior_manager.particle_type_count));
    }
}

void ParticleSimulator::process() {
    sync_settings();
    if (!paused) {
        if (uses_particle_grid) prepare_grid();

        for (size_t p_id = 0; p_id < particle_count; p_id++) {
            if (uses_particle_grid) generate_grid(p_id);
        }

        for (size_t p_id = 0; p_id < particle_count; p_id++) {
            if (!uses_terminal_velocity) apply_terminal_velocity(p_id, 1.f);
            handle_particle_velocity(p_id);
            if (uses_terminal_velocity) apply_terminal_velocity(p_id, terminal_velocity_strength);
        }

        for (size_t p_id = 0; p_id < particle_count; p_id++) {
            update_particle_position(p_id);
            handle_out_of_bounds(p_id);
        }
    }
}

void ParticleSimulator::handle_particle_velocity(size_t p_id) {
    if (uses_particle_grid) {
        std::pair<size_t, size_t> cell = convert_coords_to_cell(positions_x[p_id], positions_y[p_id]);

        int cell_radius = ceil(behavior_manager.interaction_radius/static_cast<float>(cell_size));
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
    } else {
        for (size_t p2_id = 0; p2_id < particle_count; p2_id++) {
            if (p_id != p2_id) {
                update_particle_velocity(p_id, p2_id, 0, 0);
            }
        }
    }
}

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
    float force = behavior_manager.calculate_attraction(user_interface->elements["behaviour_formula"]->value, distance, behavior_manager.particle_interaction_matrix[types[p1]][types[p2]]);
    force *= force_multiplier;
    velocities_x[p1] += force * normal_x * *delta * 60.f;
    velocities_y[p1] += force * normal_y * *delta * 60.f;

    // Max velocity
    float curr_velocity = hypot(velocities_x[p1], velocities_y[p1]);
    if (curr_velocity > max_velocity) {
        float val_scale = max_velocity / curr_velocity;
        velocities_x[p1] *= val_scale;
        velocities_y[p1] *= val_scale;
    }
}

void ParticleSimulator::update_particle_position(size_t p) {
    positions_x[p] += velocities_x[p] * *delta;
    positions_y[p] += velocities_y[p] * *delta;
}

void ParticleSimulator::prepare_grid() {
    // Clears and resizes current particle_grid
    particle_grid.resize(cell_count_y);
    for (auto& y_vec : particle_grid) {
        y_vec.resize(cell_count_x);
        for (auto& x_vec : y_vec) {
            x_vec = std::vector<size_t>();
        }
    }
}

void ParticleSimulator::generate_grid(size_t p_id) {
    // Adds particles into the grid
    std::pair<size_t, size_t> cell = convert_coords_to_cell(positions_x[p_id], positions_y[p_id]);
    particle_grid[cell.second][cell.first].push_back(p_id);
}

void ParticleSimulator::handle_out_of_bounds(size_t id) {
    if (is_space_wrapping_enabled) {
        wrap_around(id);
    } else {
        clamp(id);
    }
}

void ParticleSimulator::wrap_around(size_t p) {
    positions_x[p] = utils::abs_mod(positions_x[p], width);
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

void ParticleSimulator::drag_particles(sf::Vector2f from, sf::Vector2f to, float radius, float attraction_force, float drag_curvature, bool drag_type) {
    // Attracts particles
    for (size_t i = 0; i < particle_count; i++) {
        sf::Vector2f normal = from - sf::Vector2f(positions_x[i], positions_y[i]);
        if (is_space_wrapping_enabled) {
            normal = utils::abs_mod(normal, sf::Vector2f(width, height));
        }
        float distance = normal.length();
        if (is_space_wrapping_enabled) {
            if (abs(normal.x) > width/2.f) {
                normal.x -= utils::sign(normal.x) * width;
            }
            if (abs(normal.y) > height/2.f) {
                normal.y -= utils::sign(normal.y) * height;
            }
            distance = normal.length();
        }
        normal = distance!=0 ? normal/distance : sf::Vector2f(0.f, 0.f);
        if (!drag_type|| paused) {
            if (distance < radius) {
                positions_x[i] += pow((radius - distance)/radius, drag_curvature) * (to.x - from.x);
                positions_y[i] += pow((radius - distance)/radius, drag_curvature) * (to.y - from.y);
                handle_out_of_bounds(i);
            }
        } else {
            if (distance < radius) {
                velocities_x[i] += normal.x * min(pow(radius - distance, 4.f), attraction_force) * *delta * 60.f;
                velocities_y[i] += normal.y * min(pow(radius - distance, 4.f), attraction_force) * *delta * 60.f;
                handle_out_of_bounds(i);
            }
        }
    }
}

void ParticleSimulator::apply_terminal_velocity(size_t p, float strength) {
    float velocity_multiplier = behavior_manager.calculate_terminal_velocity_change(*delta, strength);
    velocities_x[p] *= velocity_multiplier;
    velocities_y[p] *= velocity_multiplier;
}

std::pair<size_t, size_t> ParticleSimulator::convert_coords_to_cell(float x, float y) {
    std::pair a = std::make_pair(static_cast<size_t>(floor(x/cell_size)), static_cast<size_t>(floor(y/cell_size)));
    a.first = min(a.first, static_cast<size_t>(cell_count_x-1));
    a.second = min(a.second, static_cast<size_t>(cell_count_y-1));
    return a;
}

bool ParticleSimulator::does_cell_exist(size_t x, size_t y) {
    return ceil(static_cast<float>(width)/static_cast<float>(cell_size)) > x && ceil(static_cast<float>(height)/static_cast<float>(cell_size)) > y;
}

std::vector<size_t>& ParticleSimulator::get_particles_in_cell(int x, int y) {
    if (!is_space_wrapping_enabled && !does_cell_exist(x, y)) {
        return empty_vec;
    } else {
        return particle_grid[utils::abs_mod(y, cell_count_y)][utils::abs_mod(x, cell_count_x)];
    }

}

void ParticleSimulator::spawn_particle(float x, float y, size_t count, unsigned short t) {
    for (int i = 0; i < count; i++) {
        particle_count++;
        positions_x.push_back(x + fmod(rand()/10000000.f, 1.f));
        positions_y.push_back(y + fmod(rand()/10000000.f, 1.f));
        velocities_x.push_back(0);
        velocities_y.push_back(0);
        types.push_back(fmod(t + i, behavior_manager.particle_type_count));
        handle_out_of_bounds(particle_count-1);
    }
}

void ParticleSimulator::spawn_particle(float x, float y, size_t count) {
    spawn_particle(x, y, count,rand() % behavior_manager.particle_type_count);
}

void ParticleSimulator::delete_particle(size_t id) {
    particle_count--;
    positions_x.erase(positions_x.begin() + id);
    positions_y.erase(positions_y.begin() + id);
    velocities_x.erase(velocities_x.begin() + id);
    velocities_y.erase(velocities_y.begin() + id);
    types.erase(types.begin() + id);
}

void ParticleSimulator::set_particle_count(int n) {
    if (n == particle_count) return;
    if (n <= 0) {
        particle_count = 0;
        positions_x.resize(0);
        positions_y.resize(0);
        velocities_x.resize(0);
        velocities_y.resize(0);
        types.resize(0);
        return;
    }
    if (n < particle_count) {
        particle_count = n;
        positions_x.resize(n);
        positions_y.resize(n);
        velocities_x.resize(n);
        velocities_y.resize(n);
        types.resize(n);
        return;
    }
    if (n > particle_count) {
        for (size_t i = particle_count; i < n; i++) {
            spawn_particle(rand() % width, rand() % height);
        }
        return;
    }
}

void ParticleSimulator::resize_cells(unsigned short size) {
    cell_size = size;
    cell_count_x = ceil(static_cast<float>(width)/static_cast<float>(cell_size));
    cell_count_y = ceil(static_cast<float>(height)/static_cast<float>(cell_size));
}

void ParticleSimulator::set_particle_type_count(int n) {
    if (n == behavior_manager.particle_type_count) return;
    if (n <= 0) {
        n = 1;
    }
    behavior_manager.resize_matrix(n);
    for (size_t i = 0; i < particle_count; i++) {
        types[i] = rand() % n;
    }
}

void ParticleSimulator::sync_settings() {
    set_particle_count(user_interface->elements["particle_count"]->value);
    behavior_manager.particle_interaction_matrix = user_interface->matrix->particle_interaction_matrix;
    set_particle_type_count(behavior_manager.particle_interaction_matrix[0].size());
    is_space_wrapping_enabled = user_interface->elements["wrapping"]->value;
    behavior_manager.min_distance = user_interface->elements["min_distance"]->value;
    behavior_manager.interaction_radius = user_interface->elements["interaction_radius"]->value;
    if (cell_size != user_interface->elements["interaction_radius"]->value/2.f) {
        resize_cells(user_interface->elements["interaction_radius"]->value/2.f);
    }
    force_multiplier = user_interface->elements["force_multiplier"]->value;
    uses_terminal_velocity = user_interface->elements["use_terminal_velocity"]->value;
    terminal_velocity_strength = user_interface->elements["terminal_velocity"]->value;

}
