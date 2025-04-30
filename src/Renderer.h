#pragma once
#include <random>
#include <SFML/Graphics.hpp>
#include "particles/particleSimulator.h"
#include "camera/camera.h"

using namespace std;

class Renderer {
public:
    unsigned short width, height;
    unsigned int render_fps_limit = 60;
    unsigned int physics_fps_limit = 60;
    double physics_timestamp;
    ParticleSimulator particle_simulator;
    explicit Renderer(unsigned short width = 1280, unsigned short height = 720);
    void run();
    void set_render_fps_limit(unsigned char fps);
    void set_physics_fps_limit(unsigned char fps);

private:
    sf::RenderWindow window;
    sf::Clock clock;
    Camera camera;
    sf::Vector2f global_mouse_pos;
    sf::Vector2f last_mouse_pos;
    double time{};
    bool is_focused = true;
    bool particle_drag_enabled{};
    float particle_drag_radius = 100.f;
    bool draw_particle_grid = false;
    bool draw_mouse_radius = false;

    void handle_events(const double *deltaTime);
    void render();
};