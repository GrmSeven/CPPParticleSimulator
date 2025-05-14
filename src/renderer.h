#pragma once
#include <random>
#include <SFML/Graphics.hpp>
#include "particles/particleSimulator.h"
#include "camera/camera.h"
#include "userInterface/userInterface.h"

using namespace std;

class Renderer {
public:
    unsigned short width, height;
    unsigned int fps_limit = 60;
    float delta{};
    float timer{};
    bool fullscreen{};
    ParticleSimulator particle_simulator;
    Renderer();
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
    bool is_focused = true;
    bool simulator_focused = false;
    bool particle_drag_enabled{};
    float particle_drag_radius = 100.f;
    bool draw_particle_grid = false;
    bool draw_mouse_radius = false;

    void handle_events();
    void render();
};