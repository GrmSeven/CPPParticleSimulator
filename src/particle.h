#pragma once
#include <SFML/Graphics.hpp>

class particle {
public:
    particle(sf::Vector2<float> position = {0,0}, unsigned int type = 0);
    sf::Vector2<float> position;
    sf::Vector2<float> velocity;
    unsigned int type;

    void update(float& delta);
    void clamp(sf::Vector2<unsigned int> l, sf::Vector2<unsigned int> r);
};