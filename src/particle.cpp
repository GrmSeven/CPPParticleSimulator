#include "particle.h"
#include <cmath>

particle::particle(sf::Vector2<float> position, unsigned char type) : position(position), type(type) {
}

void particle::update(float& delta) {
    position += velocity * delta;
};

// If particle is out of bounds, wraps it to the other side of the screen
void particle::clamp(sf::Vector2<unsigned int> l, sf::Vector2<unsigned int> r) {
    position.x = fmod((position.x - l.x), r.x) + l.x;
    position.y = fmod((position.y - l.y), r.y) + l.y;
}

// If particle is out of bounds, than it will be reflected

void particle::reflection(sf::Vector2<unsigned int> l, sf::Vector2<unsigned int> r) {

}