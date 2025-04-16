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
}

// Put this later into other that will have multiple different formulas
// Param_1 is for making different particles behave slightly differently, based on their type, using formula
// This one uses this formula
float calculate_attraction_life(float distance, float param_1 = 1) {
    float a = 50; // If particles are too close, then they repel slightly
    float m = 200; // How far does the attraction persist
    if (distance < a) {
         return (distance-a)/4;
    } else if (distance < (m+a)/2) {
        return (distance-a)*param_1;
    } else if (distance < m) {
        return (m-distance)*param_1;
    } else {
        return 0.f;
    }
}

// Newton formula, abit boring
float calculate_attraction_newton(float distance, float param_1 = 1) {
    return 1 / distance / distance * 10000.f * param_1;
}

float calculate_attraction_inv_newton(float distance, float param_1 = 1) {
        return distance * distance * 0.001f * param_1;
}

// Runs every frame
void renderer::process() {
    // Apply velocity
    for (int i = 0; i < particles.size(); i++) {
        for (int j = i + 1; j < particles.size(); j++) {
            sf::Vector2 transform_vector = (particles[i].position - particles[j].position);
            float distance = transform_vector.length();
            sf::Vector2 normal_vector = distance == 0.f ? sf::Vector2 {0.f, 0.f} : transform_vector / distance;
            particles[j].velocity += calculate_attraction_life(distance, 0.1f) * normal_vector;
            particles[i].velocity += calculate_attraction_life(distance, 0.1f) * (sf::Vector2 {0.f, 0.f} - normal_vector);

        }
    }


    for (particle& p : particles) {
        // p.terminal_velocity(delta, 0.1f);
        p.update(delta);
        p.reflect({0, 0}, {width, height});
        p.setVelocity({0,0});
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