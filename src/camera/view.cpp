#include "view.h"

Camera::Camera(float zoom) : zoom(zoom), position(0.f, 0.f) {}

void Camera::update(float deltaTime) {

    //position transform
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        position.y -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        position.y += speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        position.x -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        position.x += speed * deltaTime;
    }

    //zoom +
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && zoom > .1f) {
        zoom *= 0.99f;
        speed *= 0.99f;
    }

    //zoom -
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && zoom < 2.3f) {
        zoom *= 1.01f;
        speed *= 1.005f;
    }
}

//main position
sf::View Camera::GetView(sf::Vector2u windowSize) {
    sf::View view;
    view.setSize(sf::Vector2(windowSize.x * zoom, windowSize.y * zoom));
    view.setCenter(position + sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
    return view;
}
