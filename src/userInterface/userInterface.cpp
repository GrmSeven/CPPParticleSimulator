#include "userInterface.h"

UserInterface::UserInterface(sf::Vector2f windowSize) {
    view.setSize(windowSize);
    view.setCenter(windowSize / 2.f);
}

void UserInterface::render(sf::RenderWindow& window) {
    window.setView(view);

    // sf::RectangleShape rect;
    // rect.setFillColor(sf::Color::White);
    // rect.setSize(sf::Vector2f(500, 500));
    // window.draw(rect);
}
