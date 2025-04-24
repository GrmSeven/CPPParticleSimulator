#pragma once
#include <random>
#include <SFML/Graphics.hpp>
#include <vector>

#include "particle.h"

class Camera;
using namespace std;

class renderer {
public:
    explicit renderer(unsigned short width = 1280, unsigned short height = 720);
    vector<particle> particles;
    void run();
    unsigned short width, height;
    unsigned int render_fps_limit = 60;
    unsigned int physics_fps_limit = 30;
    void set_render_fps_limit(unsigned char fps);
    void set_physics_fps_limit(unsigned char fps);

private:
    sf::RenderWindow window;
    sf::Clock clock;
    double time{};
    double timestamp = 1.0/physics_fps_limit;
    void pre_process();
    void handle_events(Camera *camera, const float *deltaTime);
    void process(double delta);
    void render(double delta);


};