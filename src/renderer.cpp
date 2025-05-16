#include "renderer.h"
#include <cmath>
#include "camera/camera.h"
using namespace std;

Renderer::Renderer()
    : width(sf::VideoMode::getDesktopMode().size.x*0.9), height(sf::VideoMode::getDesktopMode().size.y - sf::VideoMode::getDesktopMode().size.x*0.1),
    user_interface(sf::Vector2f(width, height)),
    particle_simulator(2003, 2003, &delta, &user_interface),
    camera(1.8f, sf::Vector2f(2003, 2003), sf::Vector2f(width, height)),
    particle_shape(sf::PrimitiveType::Triangles, 0),
    particle_vertices(sf::PrimitiveType::Triangles, 0)
{
    // width = ;
    settings.antiAliasingLevel = 4;
    window.create(sf::VideoMode({width, height}), "Particle Simulator", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, sf::State::Windowed, settings);
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
            draw_particle_grid = false;
            draw_mouse_radius = false;
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

                // Circle around mouse
                if (keyPressed->code == sf::Keyboard::Key::LControl) {
                    draw_mouse_radius = true;
                }
                // Toggles grid
                if (keyPressed->code == sf::Keyboard::Key::LAlt) {
                    draw_particle_grid = true;
                }

                // Fullscreen
                if (keyPressed->code == sf::Keyboard::Key::F11) {
                    window.close();
                    fullscreen = !fullscreen;
                    if (fullscreen) {
                        window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().size), "Particle Simulator", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, sf::State::Fullscreen);
                        camera.resize_window(sf::Vector2f(window.getSize()));
                        user_interface.resize(sf::Vector2f(window.getSize()));
                    } else {
                        width = sf::VideoMode::getDesktopMode().size.x*0.9;
                        height = sf::VideoMode::getDesktopMode().size.y - sf::VideoMode::getDesktopMode().size.x*0.1;
                        window.create(sf::VideoMode({width, height}), "Particle Simulator", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, sf::State::Windowed);
                        camera.resize_window(sf::Vector2f(width, height));
                        user_interface.resize(sf::Vector2f(width, height));
                    }
                }

                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    if (fullscreen) {
                        window.close();
                        fullscreen = false;
                        width = sf::VideoMode::getDesktopMode().size.x*0.9;
                        height = sf::VideoMode::getDesktopMode().size.y - sf::VideoMode::getDesktopMode().size.x*0.1;
                        window.create(sf::VideoMode({width, height}), "Particle Simulator", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, sf::State::Windowed);
                        camera.resize_window(sf::Vector2f(width, height));
                        user_interface.resize(sf::Vector2f(width, height));
                    }
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
                        particle_simulator.spawn_particle(global_mouse_pos.x, global_mouse_pos.y, user_interface.elements[12]->value, user_interface.elements[13]->value);
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
                    user_interface.mouse_pressed(mouseButton->position, -1);
                }
                if (mouseButton->button == sf::Mouse::Button::Right) {
                    user_interface.mouse_pressed(mouseButton->position, 1);
                }
                if (mouseButton->button == sf::Mouse::Button::Middle) {
                    user_interface.mouse_pressed(mouseButton->position, 0);
                }
            }
            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    user_interface.mouse_released(mouseButton->position, -1);
                }
                if (mouseButton->button == sf::Mouse::Button::Right) {
                    user_interface.mouse_released(mouseButton->position, 1);
                }
                if (mouseButton->button == sf::Mouse::Button::Middle) {
                    user_interface.mouse_released(mouseButton->position, 0);
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
        if (particle_drag_enabled) {
            particle_simulator.drag_particles(last_mouse_pos, global_mouse_pos, particle_drag_radius, user_interface.elements[9]->value*10.f, 1.f-(user_interface.elements[9]->value/100.f), !user_interface.elements[7]->value);
            last_mouse_pos = global_mouse_pos;
        }

        camera.update(window, delta);
    }

}


/**
 * Renders particles and UI
 */
void Renderer::render() {
    window.clear();

    // Particle rendering
    const int vertex_count = user_interface.elements[27]->value;
    const float p_radius = user_interface.elements[28]->value;
    // Create particle shape preset
    particle_shape.resize(3*(vertex_count-2));
    for (int i = 0; i < vertex_count-2; i++) {
        const float angle = (i+1) * 6.28f / vertex_count;
        const float angle2 = (i+2) * 6.28f / vertex_count;
        particle_shape[i*3].position = sf::Vector2f(p_radius, 0);
        particle_shape[i*3+1].position = sf::Vector2f(cos(angle) * p_radius, sin(angle) * p_radius);
        particle_shape[i*3+2].position = sf::Vector2f(cos(angle2) * p_radius, sin(angle2) * p_radius);
    }
    // Draw particles
    particle_vertices.resize(3*(vertex_count-2)*particle_simulator.particle_count);
    sf::Color particle_color;
    const bool visualize_velocity = user_interface.elements[30]->value;
    for (size_t p_id = 0; p_id < particle_simulator.particle_count; p_id++) {
        sf::Vector2f shift = {particle_simulator.positions_x[p_id], particle_simulator.positions_y[p_id]};
        if (visualize_velocity) {
            const float velocity = sqrtf(particle_simulator.velocities_x[p_id] * particle_simulator.velocities_x[p_id] + particle_simulator.velocities_y[p_id] * particle_simulator.velocities_y[p_id])/200.f;
            particle_color = utils::lerp(sf::Color(0, 0, 50), sf::Color(244, 186, 29), utils::clamp(velocity, 0, 1));
        } else {
            particle_color = user_interface.matrix->get_particle_color(particle_simulator.types[p_id]);
        }
        for (int i = 0; i < vertex_count-2; i++) {
            const int index = i*3+p_id*3*(vertex_count-2);
            particle_vertices[index].position = particle_shape[i*3].position + shift;
            particle_vertices[index+1].position = particle_shape[i*3+1].position + shift;
            particle_vertices[index+2].position = particle_shape[i*3+2].position + shift;
            particle_vertices[index].color = particle_color;
            particle_vertices[index+1].color = particle_color;
            particle_vertices[index+2].color = particle_color;
        }
    }
    window.draw(particle_vertices);

    // Draw grid
    if (draw_particle_grid) {
        sf::VertexArray grid(sf::PrimitiveType::Lines, particle_simulator.cell_count_x*2 + particle_simulator.cell_count_y*2 - 4);
        float& cell_size = particle_simulator.cell_size;
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
    float time_counter = 0;
    int frame_counter = 0;
    float fps_update_time = 0.5f;
    while (window.isOpen()) {
        // Synchronises with ui
        particle_drag_radius = user_interface.elements[6]->value;
        if (user_interface.elements[14]->value != fps_limit) {
            set_fps_limit(user_interface.elements[14]->value);
        }

        delta = clock.restart().asSeconds();
        timer += delta;
        time_counter += delta;
        frame_counter++;
        if (timer >= fps_update_time) {
            timer -= fps_update_time;
            user_interface.fps_counter = static_cast<int>(1.f/(time_counter/frame_counter));
            time_counter = 0;
            frame_counter = 0;
        }
        delta = min(delta, 1.f/user_interface.elements[15]->value); // Slows down the simulation

        handle_events();
        particle_simulator.process();
        render();
    }
}

void Renderer::set_fps_limit(unsigned char fps) {
    fps_limit = fps;
    window.setFramerateLimit(fps);
}
