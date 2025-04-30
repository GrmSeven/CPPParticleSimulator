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
    sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));
    global_mouse_pos = window.mapPixelToCoords(sf::Vector2i(mouse_pos), window.getView());

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
            // Circle around mouse
            if (const auto* keyRelased = event->getIf<sf::Event::KeyReleased>()) {
                if (keyRelased->code == sf::Keyboard::Key::LControl) {
                    draw_mouse_radius = false;
                }
                if (keyRelased->code == sf::Keyboard::Key::LAlt) {
                    draw_particle_grid = false;
                }
            }

            // Particle spawning
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    particle_simulator.spawn_particle(global_mouse_pos.x, global_mouse_pos.y);
                }
            }

            // // Particle dragging
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    particle_drag_enabled = true;
                    last_mouse_pos = global_mouse_pos;
                }
            }
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    particle_drag_enabled = false;
                }
            }
            camera.handle_events(event);
            UI.handle_events(event);
        }
    }
    if (is_focused) {
        if (particle_drag_enabled) {
            particle_simulator.drag_particles(last_mouse_pos, global_mouse_pos, particle_drag_radius, 25.f, 1.f);
            last_mouse_pos = global_mouse_pos;
        }
        camera.update(window, *deltaTime);
    }

}


/**
 * Renders particles and UIx
 */
void renderer::render() {
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

    UI.render();
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