#include "renderer.h"
#include <cmath>

#include "particle.h"
using namespace std;

renderer::renderer(unsigned short width, unsigned short height)
    : window(sf::VideoMode({width, height}), "Particle Simulator", sf::Style::Titlebar | sf::Style::Close), delta(0),
      width(width), height(height) {
    window.setFramerateLimit(60);
}

/**
 * For keyboard and mouse inputs
 */
void renderer::handle_events() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

/**
 * Runs once at the start
 */
void renderer::pre_process() {
    // Creates a few particles particle
     // srand(time(0));
    const unsigned short partCount = 170;
    srand(0);
    for (unsigned short i = 0; i < partCount; ++i) {
        particles.push_back(particle({static_cast<float>(rand() % width), static_cast<float>(rand() % height)}, {0,0}, 'a'));
        particles.push_back(particle({static_cast<float>(rand() % width), static_cast<float>(rand() % height)}, {0,0}, 'b'));
    }
}
/**
 * This one uses this formula
 * @param distance
 * @param param_1 is for making different particles behave slightly differently, based on their type, using formula
 * @return
 */
float calculate_attraction_life(float distance, float param_1 = 1) {
    unsigned short r= 50; // If particles are too close, then they repel slightly
    unsigned short m = 200; // How far does the attraction persist
    if (distance < r) {
         return (distance-r)/4;
    } else if (distance < (m+r)/2) {
        return (distance-r)*param_1/10;
    } else if (distance < m) {
        return (m-distance)*param_1/10;
    } else {
        return 0.f;
    }
}

float calculate_smooth_attraction_life(float distance, float param_1 = 1) {
    unsigned short r = 50; // If particles are too close, then they repel slightly
    unsigned short m = 200; // How far does the attraction persist
    if (distance < r) {
        return (distance*distance-r*r)/r/5;
    } else if (distance < m) {
        float g = distance-r;
        return ( -g*g + g*(m-r) )/( m-r )*param_1/5;
    } else {
        return 0.f;
    }
}

/**
 * Newton formula, abit boring
 * @param distance
 * @param param_1 is for making different particles behave slightly differently, based on their type, using formula
 * @return
 */
float calculate_attraction_newton(float distance, float param_1 = 1) {
    return 1 / distance / distance * 10000.f * param_1;
}

float calculate_attraction_inv_newton(float distance, float param_1 = 1) {
        return distance * distance * 0.001f * param_1;
}

/**
 * Runs every frame
 */
void renderer::process() {
    // Apply velocity
    for (int i = 0; i < particles.size(); i++) {
        for (int j = i + 1; j < particles.size(); j++) {
            sf::Vector2 transform_vector = (particles[i].position - particles[j].position);
            float distance = transform_vector.length();
            sf::Vector2 normal_vector = distance == 0.f ? sf::Vector2 {0.f, 0.f} : transform_vector / distance;
            particles[j].velocity += calculate_smooth_attraction_life(distance, 1.f) * normal_vector;
            particles[i].velocity += calculate_smooth_attraction_life(distance, 1.f) * (sf::Vector2 {0.f, 0.f} - normal_vector);
        }
    }


    for (particle& p : particles) {
        p.update(delta);
        p.terminal_velocity(delta, 10.f);
        p.clamp({0, 0}, {width, height});
        // p.setVelocity({0,0});
    }
}

/**
 * Renders particles and UI
 */
void renderer::render() {
    window.clear();

    // Particle rendering
    for (int i = 0; i < particles.size(); i++) {
        float radius = 5.0;
        sf::CircleShape circle(radius);
        if (particles[i].type == 'a') { // Crate ParticleTypes class to lookup color, size, other parameters
            circle.setFillColor(sf::Color(255, 0, 0, 255));
        } else {
            circle.setFillColor(sf::Color(0, 0, 255, 255));
        }

        circle.setPosition(particles[i].position);
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

/**
 * 
 * @param fps limiter
 */
void renderer::set_framerate_limit(unsigned char fps) {
    window.setFramerateLimit(fps);
}