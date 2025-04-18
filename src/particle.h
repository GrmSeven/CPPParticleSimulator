#pragma once
#include <SFML/Graphics.hpp>

class particle {
public:

     sf::Vector2<float> position;
     sf::Vector2<float> velocity;
     unsigned char type;


    particle(sf::Vector2<float> position = {0,0}, sf::Vector2<float> velocity = {0,0},  unsigned char type = 0);

    void update(float& delta);


    void clamp(sf::Vector2<unsigned short> l, sf::Vector2<unsigned short> r);
    void reflect(sf::Vector2<unsigned short> l, sf::Vector2<unsigned short> r);
    void terminal_velocity(float& delta, float drag);

private:
};