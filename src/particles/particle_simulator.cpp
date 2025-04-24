#include "particle_simulator.h"

void particle_simulator::pre_process() {
    for (size_t i = 0; i < particle_count; i++) {
        positions[i] = sf::Vector2f(rand() % width, rand() % height);
        velocities[i] = sf::Vector2f(0, 0);
    }
}

void particle_simulator::process() {
}

void particle_simulator::update() {
}

void particle_simulator::clamp() {
}

void particle_simulator::reflect() {
}

void particle_simulator::terminal_velocity() {
}

void particle_simulator::slow_down_velocity() {
}
