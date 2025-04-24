#include "particle.h"
#include <cmath>
#include "../utils.h"

particle::particle(sf::Vector2<float> position, sf::Vector2<float> velocity, unsigned char type) : position(position),  velocity(velocity), type(type) {
}

void particle::update(double& delta) {
    position += static_cast<sf::Vector2<float>>(static_cast<sf::Vector2<double>>(velocity) * delta);
};

/**
 *
 * @param l left-down corner
 * @param r right-up corner
 * If particle is out of bounds, wraps it to the other side of the screen
 */
void particle::clamp(sf::Vector2<unsigned short> l, sf::Vector2<unsigned short> r) {
    position.x = fmod(position.x - l.x, r.x - l.x) + l.x; //we dont need to use abs because we havent a negativ vars
    position.y = fmod(position.y - l.y, r.y - l.y) + l.y;;
}

/**
 *
 * @param l left-down corner
 * @param r right-up corner
 * If particle is out of bounds, then it will be reflected
 */
void particle::reflect(sf::Vector2<unsigned short> l, sf::Vector2<unsigned short> r) {
    if (fmod(position.x-l.x,2.f*(r.x - l.x)) > r.x - l.x) velocity.x = -velocity.x;
    if (fmod(position.y-l.y,2.f*(r.y - l.y)) > r.y - l.y) velocity.y = -velocity.y;
    position.x = abs(fmod(position.x - 2.f * l.x + r.x, 2.f*(r.x-l.x))-r.x+l.x)+l.x;
    position.y = abs(fmod(position.y - 2.f * l.y + r.y, 2.f*(r.y-l.y))-r.y+l.y)+l.y;
}

/**
 *
 * @param delta
 * @param drag
 * Slows down the particle, not sure if its the best approach
 */
void particle::terminal_velocity(float& delta, float drag) {
    // velocity *= 1.f - exp(-drag*delta);
    velocity *= std::max(0.0f, 1.0f - drag * delta);
}

// Slows down particle slow down speed
void particle::slow_down_velocity(float &delta, float speed) {
    velocity *= pow(1/speed, delta);
}