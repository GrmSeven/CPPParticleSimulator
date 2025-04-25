#include "renderer.h"
#include <cmath>
#include <iostream>
#include "camera/view.h"
#include "mouse/mouse.h"
using namespace std;

renderer::renderer(unsigned short width, unsigned short height)
    : width(width), height(height), window(sf::VideoMode({width, height}), "Particle Simulator", sf::Style::Titlebar | sf::Style::Close), time(0) {
    window.setFramerateLimit(render_fps_limit);
}

/**
 * For keyboard and mouse inputs
 */
void renderer::handle_events(Camera *camera, const float *deltaTime, CMouse *mouse) {

    camera->update(*deltaTime);


    window.setView(camera->GetView(window.getSize()));

    mouse->mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mouse->mousePos, window.getView());

    mouse->cursorParticle->position = worldPos;

    while (std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            switch (mouseWheelScrolled->wheel)
            {
                case sf::Mouse::Wheel::Vertical:
                    if (mouseWheelScrolled->delta > 0) {
                        // you may to change val-s by himself, i have a mose trouble
                        camera->zoom += (1.2f - camera->zoom) * 0.1f;
                        camera->speed *= 1.02f;
                    } else {
                        camera->zoom -= (camera->zoom - 0.8f) * 0.1f;
                        camera->speed *= 0.8f;
                    }
                break;

                case sf::Mouse::Wheel::Horizontal:
                    //idk
                        break;
            }
        }
    }

}

/**
 * Runs once at the start
 */
void renderer::pre_process() {
    // Creates a few particles particle
     // srand(time(0));
    constexpr unsigned short partCount = 100; // count of particles
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
    float r= 50; // If particles are too close, then they repel slightly
    float m = 200; // How far does the attraction persist
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
    float r = 50; // If particles are too close, then they repel slightly
    float m = 200; // How far does the attraction persist
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
 * Newton formula, a bit boring
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
void renderer::process(double delta) {
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
        // p.terminal_velocity(delta, 1.f);
        // p.slow_down_velocity(delta, 10.f);
        p.update(delta);
        p.clamp({0, 0}, {width, height});
        p.setVelocity({0,0});
    }
}

/**
 * Renders particles and UI
 */
void renderer::render(double delta) {
    window.clear();

    // Particle rendering
    for (auto & particle : particles) {
        float radius = 5.0;
        sf::CircleShape circle(radius);
        if (particle.type == 'a') { // Crate ParticleTypes class to lookup color, size, other parameters
            circle.setFillColor(sf::Color(255, 0, 0, 255));
        } else {
            circle.setFillColor(sf::Color(0, 0, 255, 255));
        }

        circle.setPosition(particle.position);
        circle.setOrigin({radius, radius});
        window.draw(circle);
    }

    window.display();
}

// Main loop
void renderer::run() {
    pre_process();

    Camera camera;

    while (window.isOpen()) {
        particle cursorParticle = particle({0,0}, {0,0}, 'a');
        CMouse mouse{ {0,0}, &cursorParticle };

        float clockVal = clock.getElapsedTime().asSeconds(); //1 Thread var for physic, need to add independent thread for physic and camera
        camera.GetView(window.getSize()); //create a view

        window.setView(camera.GetView(window.getSize()));

        handle_events(&camera, &clockVal, &mouse); //events from user and from physics
        // time = min(clock.restart().asSeconds(), 1/static_cast<float>(framerate_limit));
        time += clockVal; // Fixed physics fps
        timestamp = 1.0/physics_fps_limit;
        if (time >= timestamp) {
            time -= timestamp;
            process(timestamp);
        }
        render(1.0/render_fps_limit);
    }

    // sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    // CMouse mouse = CMouse(mousePos);
}

/**
 * 
 * @param fps limiter
 */
void renderer::set_render_fps_limit(unsigned char fps) {
    render_fps_limit = fps;
    window.setFramerateLimit(fps);
}

void renderer::set_physics_fps_limit(unsigned char fps) {
    physics_fps_limit = fps;
}