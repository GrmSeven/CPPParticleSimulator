#include "camera.h"
#include "../utils.h"
#include <cmath>
#include <iostream>
#include <ostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Mouse.hpp"

Camera::Camera(float zoom, sf::Vector2f position, sf::Vector2f windowSize) : windowSize(windowSize) {
    view.setSize(sf::Vector2(windowSize * zoom));
    view.setCenter(position + windowSize / 2.f);
}

void Camera::update(double deltaTime) {

    //position transform
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        move_camera(0, -1, deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        move_camera(0, 1, deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        move_camera(-1, 0, deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        move_camera(1, 0, deltaTime);
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        cout << "left" << endl;
    }
}

void Camera::mouse_zoom(sf::View& view, sf::Vector2i pixel, const sf::RenderWindow& window, float zoom)
{
    const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel, view) };
    view.zoom(zoom);
    const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel, view) };
    const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
    view.move(offsetCoords);
}

void Camera::move_camera(float x, float y, double deltaTime) {
    float zoom = getViewZoom();
    view.move(sf::Vector2f(x * speed * zoom * deltaTime, y * speed * zoom * deltaTime));
}

float Camera::getViewZoom() {
    return view.getSize().x / windowSize.x;
}


