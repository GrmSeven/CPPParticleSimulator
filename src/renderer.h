#pragma once
#include <random>
#include <SFML/Graphics.hpp>
#include "particles/particle_simulator.h"
#include "mouse/mouse.h"

class Camera;
using namespace std;

class renderer {
public:
    unsigned short width, height;
    unsigned int render_fps_limit = 30;
    unsigned int physics_fps_limit = 30;
    double physics_timestamp;
    particle_simulator particle_simulator;
    explicit renderer(unsigned short width = 1280, unsigned short height = 720);
    void run();
    void set_render_fps_limit(unsigned char fps);
    void set_physics_fps_limit(unsigned char fps);

private:
    sf::RenderWindow window;
    sf::Clock clock;
    double time{};

    void pre_process();
    void handle_events(Camera *camera, const double *deltaTime);
    void process(double delta);
    void render(double delta);


};