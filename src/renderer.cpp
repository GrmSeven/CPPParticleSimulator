#include "renderer.h"
#include <cmath>
#include "camera/camera.h"
using namespace std;

Renderer::Renderer(unsigned short width, unsigned short height)
    : width(width), height(height),
    particle_simulator(width, height, &delta),
    window(sf::VideoMode({width, height}), "Particle Simulator", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize),
    camera(1.f, sf::Vector2f(0, 0), sf::Vector2f(window.getSize())),
    user_interface(sf::Vector2f(window.getSize()))
{
    window.setFramerateLimit(fps_limit);
}

/**
 * For keyboard and mouse inputs
 */
void Renderer::handle_events() {
    window.setView(camera.view);
    sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));
    global_mouse_pos = window.mapPixelToCoords(sf::Vector2i(mouse_pos), window.getView());
    simulator_focused = mouse_pos.x > user_interface.sidebar_size;

    while (std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (event->is<sf::Event::FocusLost>()) {
            is_focused = false;
            camera.is_dragging = false;
        }

        if (event->is<sf::Event::FocusGained>()) {
            is_focused = true;
        }

        if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            width = resized->size.x;
            height = resized->size.y;
            camera.resize_window(sf::Vector2f(width, height));
            user_interface.resize(sf::Vector2f(width, height));
        }

        if (is_focused) {
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // Pause
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    particle_simulator.paused = !particle_simulator.paused;
                }

                // Spawn/Despawn particles
                if (keyPressed->code == sf::Keyboard::Key::W) {
                    particle_simulator.set_particle_count(particle_simulator.particle_count + 100);
                }
                if (keyPressed->code == sf::Keyboard::Key::S) {
                    particle_simulator.set_particle_count(particle_simulator.particle_count - 100);
                }
                if (keyPressed->code == sf::Keyboard::Key::Q) {
                    particle_simulator.spawn_particle(global_mouse_pos.x, global_mouse_pos.y, 100);
                }
                if (keyPressed->code == sf::Keyboard::Key::R) {
                    particle_simulator.behavior_manager.randomize_matrix();
                }
                if (keyPressed->code == sf::Keyboard::Key::E) {
                    particle_simulator.set_particle_type_count(particle_simulator.behavior_manager.particle_type_count + 1);
                }
                if (keyPressed->code == sf::Keyboard::Key::D) {
                    particle_simulator.set_particle_type_count(particle_simulator.behavior_manager.particle_type_count - 1);
                }
                // Circle around mouse
                if (keyPressed->code == sf::Keyboard::Key::LControl) {
                    draw_mouse_radius = true;
                }
                // Toggles grid
                if (keyPressed->code == sf::Keyboard::Key::LAlt) {
                    draw_particle_grid = true;
                }
            }

            if (const auto* keyRelased = event->getIf<sf::Event::KeyReleased>()) {
                if (keyRelased->code == sf::Keyboard::Key::LControl) {
                    draw_mouse_radius = false;
                }
                if (keyRelased->code == sf::Keyboard::Key::LAlt) {
                    draw_particle_grid = false;
                }
            }

            // Particle spawning/moving
            if (simulator_focused) {
                if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseButton->button == sf::Mouse::Button::Right) {
                        particle_simulator.spawn_particle(global_mouse_pos.x, global_mouse_pos.y);
                    }
                }

                if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseButton->button == sf::Mouse::Button::Left) {
                        particle_drag_enabled = true;
                        last_mouse_pos = global_mouse_pos;
                    }
                }
            }
            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    particle_drag_enabled = false;
                }
            }

            // UI
            if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
                user_interface.mouse_moved(mouseMove->position);
            }

            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    user_interface.mouse_pressed(mouseButton->position);
                }
            }
            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    user_interface.mouse_released(mouseButton->position);
                }
            }

            if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                user_interface.mouse_scrolled(mouseWheel->position, mouseWheel->delta);
            }

            camera.is_active = simulator_focused;
            camera.handle_events(event);
        }
    }
    if (is_focused) {
        if (particle_drag_enabled && simulator_focused) {
            particle_simulator.drag_particles(last_mouse_pos, global_mouse_pos, particle_drag_radius, 25.f, 1.f);
            last_mouse_pos = global_mouse_pos;
        }
        camera.update(window, delta);
    }

}


/**
 * Renders particles and UIx
 */
void Renderer::render() {
    window.clear();

    // Particle rendering
    int vertex_count = 8;
    float p_radius = 2.f;
    // Create particle shape preset
    sf::VertexArray particle_shape(sf::PrimitiveType::Triangles, 3*(vertex_count-2));
    for (int i = 0; i < vertex_count-2; i++) {
        float angle = (i+1) * 2 * M_PI / vertex_count;
        float angle2 = (i+2) * 2 * M_PI / vertex_count;
        particle_shape[i*3].position = sf::Vector2f(p_radius, 0);
        particle_shape[i*3+1].position = sf::Vector2f(cos(angle) * p_radius, sin(angle) * p_radius);
        particle_shape[i*3+2].position = sf::Vector2f(cos(angle2) * p_radius, sin(angle2) * p_radius);
    }
    // Draw particles
    sf::VertexArray particle_vertices(sf::PrimitiveType::Triangles, 3*(vertex_count-2)*particle_simulator.particle_count);
    for (size_t p_id = 0; p_id < particle_simulator.particle_count; p_id++) {
        sf::Vector2f shift = {particle_simulator.positions_x[p_id], particle_simulator.positions_y[p_id]};
        sf::Color particle_color = particle_simulator.behavior_manager.get_particle_color(particle_simulator.types[p_id]);
        for (int i = 0; i < vertex_count-2; i++) {
            particle_vertices[i*3+p_id*3*(vertex_count-2)].position = particle_shape[i*3].position + shift;
            particle_vertices[i*3+p_id*3*(vertex_count-2)+1].position = particle_shape[i*3+1].position + shift;
            particle_vertices[i*3+p_id*3*(vertex_count-2)+2].position = particle_shape[i*3+2].position + shift;
            particle_vertices[i*3+p_id*3*(vertex_count-2)].color = particle_color;
            particle_vertices[i*3+p_id*3*(vertex_count-2)+1].color = particle_color;
            particle_vertices[i*3+p_id*3*(vertex_count-2)+2].color = particle_color;
        }
    }
    window.draw(particle_vertices);

    // Draw grid
    if (draw_particle_grid) {
        sf::VertexArray grid(sf::PrimitiveType::Lines, particle_simulator.cell_count_x*2 + particle_simulator.cell_count_y*2 - 4);
        unsigned short& cell_size = particle_simulator.cell_size;
        for (int i = 1; i < particle_simulator.cell_count_x; i++) {
            grid[i*2 - 2].position = sf::Vector2f(i*cell_size, 0.f);
            grid[i*2 - 1].position = sf::Vector2f(i*cell_size, particle_simulator.height);
        }
        for (int i = 1; i < particle_simulator.cell_count_y; i++) {
            grid[particle_simulator.cell_count_x*2 + i*2 - 4].position = sf::Vector2f(0.f, i*cell_size);
            grid[particle_simulator.cell_count_x*2 + i*2 - 3].position = sf::Vector2f(particle_simulator.width, i*cell_size);
        }
        window.draw(grid);
    }

    // Draw circle around the mouse
    if (draw_mouse_radius) {
        sf::CircleShape mouse_circle(particle_drag_radius);
        mouse_circle.setFillColor(sf::Color(0, 0, 0, 0));
        mouse_circle.setOutlineThickness(1.0f);
        mouse_circle.setOutlineColor(sf::Color(255, 255, 255));
        mouse_circle.setPointCount(64);
        mouse_circle.setPosition(global_mouse_pos - sf::Vector2f(particle_drag_radius, particle_drag_radius));
        window.draw(mouse_circle);
    }

    // User interface
    user_interface.render(window);

    window.display();
}

// Main loop
void Renderer::run() {
    // Thread for rendering
    // Thread for particle (copy settings from global settings -> run buffered methods -> particle simulation -> repeat)
    // handle events (view and particle stuff save to global settings)
    while (window.isOpen()) {
        delta = clock.restart().asSeconds();
        timer += delta;
        if (timer >= 0.25f) {
            timer -= 0.25f;
            user_interface.fps_counter = round(10.f/delta)/10.f;
        }
        delta = min(delta, 1.f/30.f);

        handle_events();
        if (true) { // REMOVE AFTER ADDING THREADS // should check if window is being dragged by checking if pollEvent worked this frame
            particle_simulator.process();
        }
        render();
    }
}

void Renderer::set_fps_limit(unsigned char fps) {
    fps_limit = fps;
    window.setFramerateLimit(fps);
}
