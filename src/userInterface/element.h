#pragma once
#include <SFML/Graphics/Rect.hpp>
#include "SFML/Graphics/RenderWindow.hpp"
#include <iostream>

using namespace std;

class Element : public sf::FloatRect {
public:
    using sf::FloatRect::FloatRect;
    Element(sf::Vector2f pos, sf::Vector2f size) {
        this->position = pos;
        this->size = size;
        normal();
    };

    sf::RectangleShape rect;

    void draw(sf::RenderWindow* window) {
        rect.setSize({size.x, size.y});
        rect.setPosition({position.x, position.y});
        window->draw(rect);
    }

    void normal() {
        // rect.setFillColor(sf::Color(150, 150, 150));
    }

    void mouse_over() {
        // rect.setFillColor(sf::Color(150, 150, 150));
    }

    void press() {
        // rect.setFillColor(sf::Color(50, 50, 50));
    }

    void click() {
        cout << "CLICK" << endl;
    };
};
