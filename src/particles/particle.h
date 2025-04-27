#pragma once
#include <SFML/Graphics.hpp>

class particle {
public:

     sf::Vector2<float> position;
     sf::Vector2<float> velocity;
     unsigned char type;


    particle(sf::Vector2<float> position = {0,0}, sf::Vector2<float> velocity = {0,0},  unsigned char type = 0);

    void update(double& delta);


    void clamp(sf::Vector2<unsigned short> l, sf::Vector2<unsigned short> r);
    void reflect(sf::Vector2<unsigned short> l, sf::Vector2<unsigned short> r);
    void terminal_velocity(float& delta, float drag);
    void slow_down_velocity(float &delta, float speed);

    [[nodiscard]] sf::Vector2<float> getPosition() const {
        return position;
    }
    [[nodiscard]] sf::Vector2<float> getVelocity() const {
        return velocity;
    }
    [[nodiscard]] unsigned char getType() const {
        return type;
    }

    void setPosition(const sf::Vector2<float> &position) {
        this->position = position;
    }

    void setVelocity(const sf::Vector2<float> &velocity) {
        this->velocity = velocity;
    }

    void setType(unsigned char type) {
        this->type = type;
    }
};