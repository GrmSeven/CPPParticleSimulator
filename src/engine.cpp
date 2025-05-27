#include "engine.h"
#include <cmath>
#include "camera/camera.h"
#include "threadManager.h"

using namespace std;

Engine::Engine()
    : width(sf::VideoMode::getDesktopMode().size.x*0.9), height(sf::VideoMode::getDesktopMode().size.y - sf::VideoMode::getDesktopMode().size.x*0.1),
    user_interface(sf::Vector2f(width, height)),
    particle_simulator(2003, 2003, &delta, &user_interface),
    camera(1.7f, sf::Vector2f(2003, 2003), sf::Vector2f(width, height)),
    particle_shape(sf::PrimitiveType::Triangles, 0),
    particle_vertices(sf::PrimitiveType::Triangles, 0)
{
    settings.antiAliasingLevel = 4;
    window.create(sf::VideoMode({width, height}), "Particle Life Simulator", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, sf::State::Windowed, settings);
    window.setFramerateLimit(fps_limit);
}

/**
 * For keyboard and mouse inputs
 */
void Engine::handle_events() {
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
            shift_pressed = false;
            camera.shift_pressed = false;
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
                    user_interface.elements[39]->value = !user_interface.elements[39]->value;
                    user_interface.elements[39]->update_shapes();
                }
                // Toggles grid
                if (keyPressed->code == sf::Keyboard::Key::LAlt) {
                    draw_particle_grid = true;
                }

                if (keyPressed->code == sf::Keyboard::Key::LAlt) {
                    draw_particle_grid = true;
                }

                // Fullscreen
                if (keyPressed->code == sf::Keyboard::Key::F11) {
                    window.close();
                    fullscreen = !fullscreen;
                    if (fullscreen) {
                        window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().size), "Particle Life Simulator", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, sf::State::Fullscreen);
                        camera.resize_window(sf::Vector2f(window.getSize()));
                        user_interface.resize(sf::Vector2f(window.getSize()));
                    } else {
                        width = sf::VideoMode::getDesktopMode().size.x*0.9;
                        height = sf::VideoMode::getDesktopMode().size.y - sf::VideoMode::getDesktopMode().size.x*0.1;
                        window.create(sf::VideoMode({width, height}), "Particle Life Simulator", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, sf::State::Windowed);
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
                        window.create(sf::VideoMode({width, height}), "Particle Life Simulator", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, sf::State::Windowed);
                        camera.resize_window(sf::Vector2f(width, height));
                        user_interface.resize(sf::Vector2f(width, height));
                    }
                }
            }

            if (const auto* keyRelased = event->getIf<sf::Event::KeyPressed>()) {
                // Shift
                if (keyRelased->code == sf::Keyboard::Key::LShift) {
                    shift_pressed = true;
                }
                // Brush circle
                if (keyRelased->code == sf::Keyboard::Key::LControl) {
                    user_interface.elements[35]->value = !user_interface.elements[35]->value;
                    user_interface.elements[35]->update_shapes();
                }
                //// UI shortcuts
                // Particle count
                for (int i = 0; i < (shift_pressed ? 10 : 1); i++) {
                    if (keyRelased->code == sf::Keyboard::Key::W) {
                        user_interface.elements[0]->scroll_down();
                    }
                    if (keyRelased->code == sf::Keyboard::Key::S) {
                        user_interface.elements[0]->scroll_up();
                    }
                    // Particle type count
                    if (keyRelased->code == sf::Keyboard::Key::Q) {
                        user_interface.elements[1]->scroll_down();
                    }
                    if (keyRelased->code == sf::Keyboard::Key::A) {
                        user_interface.elements[1]->scroll_up();
                    }
                    // Preset
                    if (keyRelased->code == sf::Keyboard::Key::E) {
                        user_interface.elements[3]->scroll_down();
                    }
                    if (keyRelased->code == sf::Keyboard::Key::D) {
                        user_interface.elements[3]->scroll_up();
                    }
                }
                if (keyRelased->code == sf::Keyboard::Key::R) {
                    user_interface.elements[4]->click_left();
                }
            }

            if (const auto* keyRelased = event->getIf<sf::Event::KeyReleased>()) {
                if (keyRelased->code == sf::Keyboard::Key::LAlt) {
                    draw_particle_grid = false;
                }
                if (keyRelased->code == sf::Keyboard::Key::LShift) {
                    shift_pressed = false;
                }
            }


            // Particle spawning/moving
            if (simulator_focused) {
                if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseButton->button == sf::Mouse::Button::Right) {
                        particle_delete_enabled = true;
                        if (user_interface.elements[10]->value == 0) {
                            particle_simulator.spawn_particle(global_mouse_pos.x, global_mouse_pos.y, user_interface.elements[12]->value, user_interface.elements[6]->value, user_interface.elements[13]->value);
                            user_interface.elements[0]->value += user_interface.elements[12]->value;
                            user_interface.elements[0]->update_shapes();
                        }
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
                if (mouseButton->button == sf::Mouse::Button::Right) {
                    particle_delete_enabled = false;
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
                cout << mouseWheel->delta << endl;
                scroll_counter += abs(mouseWheel->delta);
                if (scroll_counter > 1) {
                    scroll_counter -= 1;
                    if (mouseWheel->position.x < user_interface.sidebar_size) {
                        user_interface.mouse_scrolled(mouseWheel->position, mouseWheel->delta, shift_pressed);
                    } else {
                        if (shift_pressed) {
                            for (int i = 0; i < 5; i++) {
                                if (mouseWheel->delta > 0) {
                                    user_interface.elements[6]->scroll_down();
                                } else {
                                    user_interface.elements[6]->scroll_up();
                                }
                            }
                            user_interface.elements[35]->value = true;
                            user_interface.elements[35]->update_shapes();
                        }
                    }
                }

            }
            camera.is_active = simulator_focused;
            camera.handle_events(event);
        }
    }
    if (is_focused) {
        if (particle_drag_enabled) {
            particle_simulator.drag_particles(last_mouse_pos, global_mouse_pos, particle_drag_radius, user_interface.elements[9]->value*10.f, 1.f-(user_interface.elements[9]->value/100.f), user_interface.elements[7]->value);
            last_mouse_pos = global_mouse_pos;
        }
        if (particle_delete_enabled && !user_interface.elements[10]->value == 0) {
            particle_simulator.delete_particle_near(global_mouse_pos, user_interface.elements[6]->value);
        }
        camera.update(window, delta);
    }
}


/**
 * Renders particles and UI
 */
void Engine::render() {
    window.clear();

    //// Particle rendering
    const int vertex_count = user_interface.elements[27]->value;
    const float p_radius = user_interface.elements[28]->value;
    // Create particle shape preset
    particle_shape.resize(3*(vertex_count-2));
    for (int i = 0; i < vertex_count-2; i++) {
        const float angle = (i+1) * 6.28f / vertex_count;
        const float angle2 = (i+2) * 6.28f / vertex_count;
        particle_shape[i*3].position = sf::Vector2f(0, -p_radius);
        particle_shape[i*3+1].position = sf::Vector2f(sin(angle) * p_radius, -cos(angle) * p_radius);
        particle_shape[i*3+2].position = sf::Vector2f(sin(angle2) * p_radius, -cos(angle2) * p_radius);
    }
    // Draw particles - clones shape preset
    const int draw_rec = user_interface.elements[40]->value;
    if (draw_rec == 2) {
        particle_vertices.resize(3*(vertex_count-2)*particle_simulator.particle_count);
    } else {
        particle_vertices.resize(3*(vertex_count-2)*particle_simulator.particle_count*9);
    }
    sf::Vector2f s_s = sf::Vector2f(particle_simulator.width, particle_simulator.height); // Simulator size
    vector<sf::Vector2f> rec_shift = {{0, 0}, {s_s.x, 0}, {-s_s.x, 0}, {0, s_s.y}, {0, -s_s.y}, {s_s.x, s_s.y}, {-s_s.x, s_s.y}, {s_s.x, -s_s.y}, {-s_s.x, -s_s.y}};
    sf::Color particle_color;
    const float visualize_velocity = user_interface.elements[30]->value;
    if (particle_simulator.use_multithreading) {
        auto func = [=](int p_id) {draw_particle(p_id, visualize_velocity, particle_color, draw_rec, vertex_count, rec_shift);};
        threadManager::multithread_range([=](size_t p_id) {func(p_id);}, 0, particle_simulator.particle_count);
    } else {
        for (size_t p_id = 0; p_id < particle_simulator.particle_count; p_id++) {
            draw_particle(p_id, visualize_velocity, particle_color, draw_rec, vertex_count, rec_shift);
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
        if (user_interface.elements[35]->value) {
            sf::CircleShape mouse_circle(particle_drag_radius);
            mouse_circle.setFillColor(sf::Color(0, 0, 0, 0));
            mouse_circle.setOutlineThickness(log2f(camera.zoom + 1));
            mouse_circle.setOutlineColor(sf::Color(255, 255, 255));
            mouse_circle.setPointCount(128);
            mouse_circle.setPosition(global_mouse_pos - sf::Vector2f(particle_drag_radius, particle_drag_radius));
            window.draw(mouse_circle);
        }

        // Drawing border
        if (user_interface.elements[25]->value) {
            sf::RectangleShape border;
            border.setSize(sf::Vector2f(particle_simulator.width, particle_simulator.height));
            border.setFillColor(sf::Color(0, 0, 0, 0));
            border.setOutlineThickness(log2f(camera.zoom + 1));
            border.setOutlineColor(sf::Color(255, 255, 255));
            window.draw(border);
        }

        // User interface
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1)) {
            user_interface.render(window);
        }

    window.display();
}

/*
 * Particle rendering for multithreading
 */
void Engine::draw_particle(size_t p_id, float visualize_velocity, sf::Color particle_color, int draw_rec, int vertex_count, vector<sf::Vector2f> rec_shift) {
    sf::Vector2f shift = {particle_simulator.positions_x[p_id], particle_simulator.positions_y[p_id]};
    if (visualize_velocity != 0) {
        const float velocity = sqrtf(particle_simulator.velocities_x[p_id] * particle_simulator.velocities_x[p_id] + particle_simulator.velocities_y[p_id] * particle_simulator.velocities_y[p_id]);
        const float scaled_velocity = velocity/visualize_velocity;
        particle_color = utils::lerp(sf::Color(0, 0, 50), sf::Color(244, 186, 29), utils::clamp(scaled_velocity, 0, 1));
    } else {
        particle_color = user_interface.matrix->get_particle_color(particle_simulator.types[p_id]);
    }
    for (int s = 0; s < (draw_rec == 2 ? 1 : 9); s++) {
        const int s_index = 3*(vertex_count-2)*particle_simulator.particle_count * s;
        sf::Color new_color = draw_rec == 1 || s == 0 ? particle_color : sf::Color::White;
        for (int i = 0; i < vertex_count-2; i++) {
            const int index = i*3+p_id*3*(vertex_count-2);
            particle_vertices[s_index+index].position = particle_shape[i*3].position + shift + rec_shift[s];
            particle_vertices[s_index+index+1].position = particle_shape[i*3+1].position + shift + rec_shift[s];
            particle_vertices[s_index+index+2].position = particle_shape[i*3+2].position + shift + rec_shift[s];
            particle_vertices[s_index+index].color = new_color;
            particle_vertices[s_index+index+1].color = new_color;
            particle_vertices[s_index+index+2].color = new_color;
        }
    }
}

// Main loop
void Engine::run() {
    float time_counter = 0;
    int frame_counter = 0;
    float fps_update_time = 0.5f;
    while (window.isOpen()) {
        // Synchronises with ui (redundant, since async multithreading isn't used)
        particle_drag_radius = user_interface.elements[6]->value;
        if (user_interface.elements[14]->value != fps_limit) {
            set_fps_limit(user_interface.elements[14]->value);
        }

        // Calculates delta and handles fps
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
        delta = min(delta, 1.f/user_interface.elements[15]->value); // Slows down the simulation when it doesn't keep up

        // Main methods
        handle_events();
        particle_simulator.process();
        render();
    }
    window.close();
}

void Engine::set_fps_limit(unsigned char fps) {
    fps_limit = fps;
    window.setFramerateLimit(fps);
}
