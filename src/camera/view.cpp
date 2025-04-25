#include "view.h"

Camera::Camera(float zoom) : zoom(zoom), position(0.f, 0.f) {}

void Camera::update(double deltaTime) {

    //position transform
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        position.y -= speed * zoom * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        position.y += speed * zoom * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        position.x -= speed * zoom * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        position.x += speed * zoom * deltaTime;
    }
}

//main position
sf::View Camera::GetView(sf::Vector2u windowSize) {
    sf::View view;
    view.setSize(sf::Vector2(windowSize.x * zoom, windowSize.y * zoom));
    view.setCenter(position + sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
    return view;
}
