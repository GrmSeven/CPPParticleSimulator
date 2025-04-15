#include "renderer.h"
#include <cmath>
#include <iostream>

#include "particle.h"
using namespace std;

renderer::renderer(unsigned int width, unsigned int height)
    : window(sf::VideoMode({width, height}), "Particle Simulator", sf::Style::Titlebar | sf::Style::Close), width(width), height(height) {
    window.setFramerateLimit(60);
}

// For keyboard and mouse inputs
void renderer::handle_events() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

// Runs once at the start
void renderer::pre_process() {
    // Creates a few particles particle
    srand(time(0));
    for (int i = 0; i < 500; ++i) {
        particles.push_back(particle({static_cast<float>(rand() % width), static_cast<float>(rand() % height)}));
    }
    // particles.push_back(particle({100.0, 300.0}, {-100, 0}));
    // particles.push_back(particle({500.0, 500.0}, {0, 0}));
    // particles.push_back(particle({200.0, 500.0}, {0, 0}));
    // particles.push_back(particle({1000.0, 500.0}, {0, -500}));
}

// Put this later into other that will have multiple different formulas
// Param_1 is for making different particles behave slightly differently, based on their type, using formula
// This one uses this formula
sf::Vector2<float> calculate_attraction_life(sf::Vector2<float> particle, sf::Vector2<float> attractor, float param_1 = 1) {
    sf::Vector2<float> direction_vector = (attractor - particle);
    sf::Vector2<float> normalized = (direction_vector.length() == 0.f) ? sf::Vector2<float>{0,0} : direction_vector / direction_vector.length();
    return normalized * (direction_vector.length());
}

// Newton formula, abit boring
sf::Vector2<float> calculate_attraction_newton(sf::Vector2<float> particle, sf::Vector2<float> attractor, float param_1 = 1) {
    sf::Vector2<float> direction_vector = (attractor - particle);
    sf::Vector2<float> normalized = (direction_vector.length() == 0.f) ? sf::Vector2<float>{0,0} : direction_vector / direction_vector.length();
    return normalized / max(direction_vector.lengthSquared(), 10.f)*10000.f*param_1;
}

sf::Vector2<float> calculate_attraction_inv_newton(sf::Vector2<float> particle, sf::Vector2<float> attractor, float param_1 = 1) {
    sf::Vector2<float> direction_vector = (attractor - particle);
    sf::Vector2<float> normalized = (direction_vector.length() == 0.f) ? sf::Vector2<float>{0,0} : direction_vector / direction_vector.length();
    return normalized * direction_vector.lengthSquared()*0.000001f;
}

// Runs every frame
void renderer::process() {
    for (int i = 0; i < particles.size(); i++) {
        // Apply velocity
        for (int j = i + 1; j < particles.size(); j++) {
            particles[i].velocity += calculate_attraction_inv_newton(particles[i].position, particles[j].position, 1);
            particles[j].velocity += calculate_attraction_inv_newton(particles[j].position, particles[i].position, 1);
        }
    }
    for (particle& p : particles) {
        // p.terminal_velocity(delta, 0.1f);
        p.update(delta);
        p.reflect({0, 0}, {width, height});
    }
}

// Renders particles and UI
void renderer::render() {
    window.clear();

    // Particle rendering
    for (particle& p : particles) {
        float radius = 5.0;
        sf::CircleShape circle(radius);
        circle.setFillColor(sf::Color(255, 0, 0, 255)); // Crate ParticleTypes class to lookup color, size, other parameters
        circle.setPosition(p.position);
        circle.setOrigin({radius, radius});
        window.draw(circle);
    }

    // UI rendering
    // const sf::Font font("hih.ttf");
    // sf::Text text(font, "Hello SFML test", 50);
    // text.setFillColor(sf::Color(25, 255, 255));
    // window.draw(text);

    window.display();
}

// Main loop
void renderer::run() {
    pre_process();
    while (window.isOpen()) {
        handle_events();
        delta = clock.restart().asSeconds();
        process();
        render();
    }
}

void renderer::set_framerate_limit(unsigned int fps) {
    window.setFramerateLimit(fps);
}