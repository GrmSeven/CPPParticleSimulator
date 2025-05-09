#include "userInterface.h"

#include <iostream>
#include <cmath>
#include <regex>

using namespace std;

UserInterface::UserInterface(sf::Vector2f windowSize) {
    view.setSize(windowSize);
    view.setCenter(windowSize / 2.f);
}

void UserInterface::render(sf::RenderWindow& window) {
    window.setView(view);
    mouse_pos = sf::Mouse::getPosition(window);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (!mouse_down) {
            mouse_just_pressed = true;
        } else {
            mouse_just_pressed = false;
        }
        mouse_down = true;
    } else {
        if (mouse_down) {
            mouse_just_relased = true;
        } else {
            mouse_just_relased = false;
        }
        mouse_down = false;
    }
    if (mouse_just_pressed) { cout << "press" << endl;}
    if (mouse_just_relased) { cout << "relase" << endl;}
    // sf::RectangleShape rect;
    // rect.setFillColor(sf::Color(20, 20, 20));
    // rect.setSize(sf::Vector2f(200, 4000));
    // window.draw(rect);





    // FPS counter
    sf::Font font("hih.ttf");
    sf::Text text(font);

    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << fps_counter;
    text.setString(stream.str());

    text.setCharacterSize(20);

    text.setPosition({view.getSize().x - text.getLocalBounds().size.x - 10.f, 0});


    text.setFillColor(sf::Color(255, 255, 255));
    text.setStyle(sf::Text::Bold);

    window.draw(text);
}

void UserInterface::resize(sf::Vector2f newWindowSize) {
    view.setSize(newWindowSize);
    view.setCenter(newWindowSize / 2.f);
}
