#include "particle.h"
#include "utils.h"
#include <cmath>
#include <iostream>

particle::particle(sf::Vector2<float> position, sf::Vector2<float> velocity, unsigned char type) : position(position),  velocity(velocity), type(type) {
}

void particle::update(float& delta) {
    position += velocity * delta;
};

// If particle is out of bounds, wraps it to the other side of the screen
void particle::clamp(sf::Vector2<unsigned int> l, sf::Vector2<unsigned int> r) {
    position.x = utils::abs_mod(position.x - l.x, r.x - l.x) + l.x;
    position.y = utils::abs_mod(position.y - l.y, r.y - l.y) + l.y;;
}

// If particle is out of bounds, then it will be reflected
void particle::reflect(sf::Vector2<unsigned int> l, sf::Vector2<unsigned int> r) {
    if (utils::abs_mod(position.x-l.x,2.f*(r.x - l.x)) > r.x - l.x) velocity.x = -velocity.x;
    if (utils::abs_mod(position.y-l.y,2.f*(r.y - l.y)) > r.y - l.y) velocity.y = -velocity.y;
    position.x = abs(utils::abs_mod(position.x - 2.f * l.x + r.x, 2.f*(r.x-l.x))-r.x+l.x)+l.x;
    position.y = abs(utils::abs_mod(position.y - 2.f * l.y + r.y, 2.f*(r.y-l.y))-r.y+l.y)+l.y;
}


