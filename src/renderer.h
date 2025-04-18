#pragma once
#include <random>
#include <SFML/Graphics.hpp>
#include <vector>

#include "particle.h"

using namespace std;

class renderer {
public:
    renderer(unsigned short width = 1280, unsigned short height = 720);
    vector<particle> particles;
    void run();
    unsigned int width, height;
    unsigned int framerate_limit = 30;
    void set_framerate_limit(unsigned int fps);

private:
    sf::RenderWindow window;
    sf::Clock clock;
    float delta;
    void pre_process();
    void handle_events();
    void process();
    void render();


};