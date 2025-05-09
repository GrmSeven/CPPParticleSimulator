#pragma once
#include <SFML/Graphics.hpp>
#include "SFML/Graphics/View.hpp"

class UserInterface {
public:
    sf::View view;
    float fps_counter;

    sf::Vector2i mouse_pos;
    bool mouse_down;
    bool mouse_just_pressed = false;
    bool mouse_just_relased = false;

    UserInterface(sf::Vector2f windowSize);
    void render(sf::RenderWindow& window);
    void resize(sf::Vector2f newWindowSize);
};

