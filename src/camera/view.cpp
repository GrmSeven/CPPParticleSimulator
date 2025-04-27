#include "view.h"
#include "../utils.h"
#include <cmath>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Mouse.hpp"

Camera::Camera(float zoom) : zoom(zoom), position(0.f, 0.f) {}

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
}

void Camera::zoomViewAt(sf::View& view, sf::Vector2i pixel, const sf::RenderWindow& window, float zoom)
{
    const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel, view) };
    view.zoom(zoom);
    const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel, view) };
    const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
    view.move(offsetCoords);
}

void Camera::move_camera(float x, float y, double deltaTime) {
    position.x += x * speed * zoom * deltaTime;
    position.y += y * speed * zoom * deltaTime;
}

void Camera::change_zoom(float step) {
    zoom = utils::clamp(zoom * pow(zoom_speed, step), 0.1f, 3.f);
}

//main position
sf::View Camera::GetView(sf::Vector2u windowSize) {
    sf::View view;
    view.setSize(sf::Vector2(windowSize.x * zoom, windowSize.y * zoom));
    view.setCenter(position + sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
    return view;
}
