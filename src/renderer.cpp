#include "renderer.h"

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
    // Creates a single particle
    particles.push_back(particle({0.0, 400.0}, 0));
}

// Runs every frame
void renderer::process() {
    for (particle& p : particles) {
        // Apply velocity
        p.velocity += {1,  2};
        p.clamp({0, 0}, {width, height});
        p.update(delta);
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