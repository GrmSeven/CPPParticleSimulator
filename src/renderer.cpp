#include "renderer.h"
#include <cmath>
#include "camera/camera.h"
using namespace std;

renderer::renderer(unsigned short width, unsigned short height)
    : physics_timestamp(1.0/physics_fps_limit), width(width), height(height),
    particle_simulator(width, height, 25, 1000, &physics_timestamp),
    window(sf::VideoMode({width, height}), "Particle Simulator", sf::Style::Titlebar | sf::Style::Close),
    camera(1.f, sf::Vector2f(0, 0), sf::Vector2f(window.getSize()))
{
    window.setFramerateLimit(render_fps_limit);
}

/**
 * For keyboard and mouse inputs
 */
void renderer::handle_events(const double *deltaTime) {
    window.setView(camera.view);
    camera.update(window, *deltaTime);

    auto mouse_pos = sf::Mouse::getPosition(window);
    sf::Vector2f global_mouse_pos = window.mapPixelToCoords(mouse_pos, window.getView());

    while (std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        // Mouse press
        if (event->is<sf::Event::MouseButtonPressed>()) {
            particle_simulator.spawn_particle(global_mouse_pos.x, global_mouse_pos.y, 'a');
        }
    }

}


/**
 * Renders particles and UI
 */
void renderer::render() {
    window.clear();

    // Particle rendering
    int vertex_count = 8;
    float p_radius = 2.f;
    sf::VertexArray particle_vertices(sf::PrimitiveType::Triangles, 3*(vertex_count-2)*particle_simulator.particle_count);
    for (size_t p_id = 0; p_id < particle_simulator.particle_count; p_id++) {
        sf::Vector2f shift = {particle_simulator.positions_x[p_id], particle_simulator.positions_y[p_id]};
        sf::Color particle_color = particle_simulator.types[p_id] == 'a' ? sf::Color::Red : sf::Color::Blue;
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