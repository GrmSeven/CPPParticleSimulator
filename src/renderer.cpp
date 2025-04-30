#include "renderer.h"
#include <cmath>
#include "camera/camera.h"
using namespace std;

renderer::renderer(unsigned short width, unsigned short height)
    : physics_timestamp(1.0/physics_fps_limit), width(width), height(height),
    particle_simulator(width, height, 25, 1000, &physics_timestamp),
    window(sf::VideoMode({width, height}), "Particle Simulator", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize),
    camera(1.f, sf::Vector2f(0, 0), sf::Vector2f(window.getSize()))
{
    window.setFramerateLimit(render_fps_limit);
}

/**
 * For keyboard and mouse inputs
 */
void renderer::handle_events(const double *deltaTime) {
    window.setView(camera.view);


    auto mouse_pos = sf::Mouse::getPosition(window);
    sf::Vector2f global_mouse_pos = window.mapPixelToCoords(mouse_pos, window.getView());

    while (std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }


        if (const auto* spacePressed = event->getIf<sf::Event::KeyPressed>()) {
            // Pause
            if (spacePressed->code == sf::Keyboard::Key::Space) {
                particle_simulator.paused = !particle_simulator.paused;
            }

            // Spawn/Despawn particles
            if (spacePressed->code == sf::Keyboard::Key::W) {
                particle_simulator.set_particle_count(particle_simulator.particle_count + 100);
            }
            if (spacePressed->code == sf::Keyboard::Key::S) {
                particle_simulator.set_particle_count(particle_simulator.particle_count - 100);
            }
            if (spacePressed->code == sf::Keyboard::Key::Q) {
                particle_simulator.spawn_particle(global_mouse_pos.x, global_mouse_pos.y, 100);
            }
            if (spacePressed->code == sf::Keyboard::Key::R) {
                particle_simulator.behavior_manager.randomize_matrix();
            }
            if (spacePressed->code == sf::Keyboard::Key::E) {
                particle_simulator.set_particle_type_count(particle_simulator.behavior_manager.particle_type_count + 1);
            }
            if (spacePressed->code == sf::Keyboard::Key::D) {
                particle_simulator.set_particle_type_count(particle_simulator.behavior_manager.particle_type_count - 1);
            }
        }

        // Particle spawning
        if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButtonPressed->button == sf::Mouse::Button::Right)
            {
                particle_simulator.spawn_particle(global_mouse_pos.x, global_mouse_pos.y);
            }
        }


        // // Particle dragging
        // if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        // {
        //     if (mouseButtonPressed->button == sf::Mouse::Button::Left)
        //     {
        //     }
        // }
        // if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonReleased>())
        // {
        //     if (mouseButtonPressed->button == sf::Mouse::Button::Left)
        //     {
        //     }
        // }

        camera.handle_events(event);
    }
    camera.update(window, *deltaTime);
}


/**
 * Renders particles and UI
 */
void renderer::render() {
    window.clear();

    // Particle rendering
    int vertex_count = 4;
    float p_radius = 2.f;
    sf::VertexArray particle_vertices(sf::PrimitiveType::Triangles, 3*(vertex_count-2)*particle_simulator.particle_count);
    for (size_t p_id = 0; p_id < particle_simulator.particle_count; p_id++) {
        sf::Vector2f shift = {particle_simulator.positions_x[p_id], particle_simulator.positions_y[p_id]};
        sf::Color particle_color = particle_simulator.behavior_manager.get_particle_color(particle_simulator.types[p_id]);
        for (int i = 0; i < vertex_count-2; i++) {
            float angle = (i+1) * 2 * M_PI / vertex_count;
            float angle2 = (i+2) * 2 * M_PI / vertex_count;
            particle_vertices[i*3+p_id*3*(vertex_count-2)].position = sf::Vector2f(p_radius, 0) + shift;
            particle_vertices[i*3+p_id*3*(vertex_count-2)+1].position = sf::Vector2f(cos(angle) * p_radius, sin(angle) * p_radius) + shift;
            particle_vertices[i*3+p_id*3*(vertex_count-2)+2].position = sf::Vector2f(cos(angle2) * p_radius, sin(angle2) * p_radius) + shift;
            particle_vertices[i*3+p_id*3*(vertex_count-2)].color = particle_color;
            particle_vertices[i*3+p_id*3*(vertex_count-2)+1].color = particle_color;
            particle_vertices[i*3+p_id*3*(vertex_count-2)+2].color = particle_color;
        }
    }
    window.draw(particle_vertices);

    window.display();
}

// Main loop
void renderer::run() {
    physics_timestamp = 1.0/physics_fps_limit;
    // srand(0);
    particle_simulator.pre_process();
    while (window.isOpen()) {
        double delta = clock.restart().asSeconds();
        time += delta; // Fixed physics fps

        handle_events(&delta);
        if (time >= physics_timestamp) {
            time -= physics_timestamp;
            particle_simulator.process();
            // cout << 1.f/delta << endl;
        }
        render();
    }
}

void renderer::set_render_fps_limit(unsigned char fps) {
    render_fps_limit = fps;
    window.setFramerateLimit(fps);
}

void renderer::set_physics_fps_limit(unsigned char fps) {
    physics_fps_limit = fps;
}