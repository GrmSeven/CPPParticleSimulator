#pragma once
#include <SFML/Graphics.hpp>

class particle {
public:

    //THIS MUST BE INCAPSULATED
    sf::Vector2<float> position;
    // float position[2]; will be better - Vector2 is a hard structure for saving a position
    sf::Vector2<float> velocity;
    unsigned char type; //int -> char dont need so many options
    //THIS MUST BE INCAPSULATED


    particle(sf::Vector2<float> position = {0,0}, unsigned char type = 0);

    void update(float& delta);
    void clamp(sf::Vector2<unsigned int> l, sf::Vector2<unsigned int> r);

    void reflection(sf::Vector2<unsigned int> l, sf::Vector2<unsigned int> r);

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

private:
    // sf::Vector2<float> position;
    // // float position[2]; will be better - Vector2 is a hard structure for saving a position
    // sf::Vector2<float> velocity;
    // unsigned char type; //int -> char dont need so many options
};