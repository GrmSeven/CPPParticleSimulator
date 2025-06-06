#pragma once
#include <random>
#include <SFML/Graphics.hpp>
#include "particles/particleSimulator.h"
#include "camera/camera.h"
#include "userInterface/userInterface.h"

using namespace std;

class Engine {
public:
    unsigned short width, height;
    unsigned int fps_limit = 60;
    float delta{};
    float timer{};
    bool fullscreen{};
    ParticleSimulator particle_simulator;
    Engine();
    void run();
    void set_fps_limit(unsigned char fps);

private:
    sf::RenderWindow window;
    sf::Clock clock;
    Camera camera;
    UserInterface user_interface;
    sf::Vector2f global_mouse_pos;
    sf::Vector2f last_mouse_pos;
    sf::ContextSettings settings;
    sf::VertexArray particle_shape;
    sf::VertexArray particle_vertices;
    bool is_focused = true;
    bool simulator_focused = false;
    bool particle_drag_enabled{};
    bool particle_delete_enabled{};
    bool shift_pressed{};
    float particle_drag_radius{};
    bool draw_particle_grid = false;
    float scroll_counter{};
    void draw_particle(size_t p_id, float visualize_velocity, sf::Color particle_color, int draw_rec, int vertex_count, vector<sf::Vector2f> rec_shift);

    void handle_events();
    void render();
};