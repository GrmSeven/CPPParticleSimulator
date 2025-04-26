#include "renderer.h"
#include <cmath>
#include <iostream>
#include "camera/view.h"
using namespace std;

renderer::renderer(unsigned short width, unsigned short height)
    : physics_timestamp(1.0/physics_fps_limit), width(width), height(height),
      particle_simulator(width, height, 50, 5000, &physics_timestamp),
      window(sf::VideoMode({width, height}), "Particle Simulator", sf::Style::Titlebar | sf::Style::Close) {
    window.setFramerateLimit(render_fps_limit);
}

/**
 * For keyboard and mouse inputs
 */
void renderer::handle_events(Camera *camera, const double *deltaTime) {
    camera->update(*deltaTime);


    window.setView(camera->GetView(window.getSize()));

    // mouse->mousePos = sf::Mouse::getPosition(window);
    // sf::Vector2f worldPos = window.mapPixelToCoords(mouse->mousePos, window.getView());

    // mouse->cursorParticle->position = worldPos;

    while (std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        //if wheel has been scrolled
        else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            switch (mouseWheelScrolled->wheel) //check type of scroll
            {
                case sf::Mouse::Wheel::Vertical: //if it was a zoom start tot change zoom
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

        // if (event->is<sf::Event::MouseButtonReleased>()){
        //     particles.push_back(*mouse->cursorParticle);
        // }

    }
}

/**
 * Runs once at the start
 */
void renderer::pre_process() {
    srand(0);
    particle_simulator.pre_process();
}
/**
 * Runs every frame
 */
void renderer::process(double delta) {
    particle_simulator.process();
}

/**
 * Renders particles and UI
 */
void renderer::render(double delta) {
    window.clear();

    // Particle rendering
    for (size_t p_id = 0; p_id < particle_simulator.particle_count; p_id++) {
        float radius = 1.0;
        sf::CircleShape circle(radius);

        circle.setFillColor(sf::Color(255, 0, 0, 255));

        circle.setPosition(sf::Vector2f{particle_simulator.positions_x[p_id], particle_simulator.positions_y[p_id]});
        circle.setOrigin({radius, radius});
        window.draw(circle);
    }

    window.display();
}

// Main loop
void renderer::run() {
    physics_timestamp = 1.0/physics_fps_limit;
    pre_process();

    Camera camera;

    while (window.isOpen()) {
        double delta = clock.restart().asSeconds();
        time += delta; // Fixed physics fps

        // Mouse and camera
        // particle cursorParticle = particle({0,0}, {0,0}, 'a');
        // CMouse mouse{ {0,0}, &cursorParticle };
        camera.GetView(window.getSize()); //create a view
        window.setView(camera.GetView(window.getSize()));

        handle_events(&camera, &delta);
        if (time >= physics_timestamp) {
            time -= physics_timestamp;
            process(physics_timestamp);
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