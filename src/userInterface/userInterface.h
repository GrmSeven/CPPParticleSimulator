#pragma once
#include <SFML/Graphics.hpp>
#include "SFML/Graphics/View.hpp"

class UserInterface {
public:
    sf::View view;

    UserInterface(sf::Vector2f windowSize);
    void render(sf::RenderWindow& window);
};

