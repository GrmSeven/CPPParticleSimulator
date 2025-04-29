#include "camera.h"
#include "../utils.h"
#include <cmath>
#include <iostream>
#include <ostream>

#include "../../cmake-build-debug/_deps/sfml-src/src/SFML/Window/InputImpl.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Mouse.hpp"

Camera::Camera(float zoom, sf::Vector2f position, sf::Vector2f windowSize) : windowSize(windowSize), zoom(zoom), wanted_zoom(zoom) {
    view.setSize(sf::Vector2(windowSize * zoom));
    view.setCenter(position + windowSize / 2.f);
}

void Camera::handle_events(std::optional<sf::Event>& event) {
    // But stuff here from renderer later for organisation
}

void Camera::update(sf::RenderWindow& window, double deltaTime) {
    if (is_dragging) {
        auto mouse_pos = sf::Mouse::getPosition(window);
        move_camera(prev_mouse_pos.x - mouse_pos.x, prev_mouse_pos.y - mouse_pos.y, 1.f);
        prev_mouse_pos = mouse_pos;
    }

    //position transform
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)
        move_camera(0, -speed, deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)
        move_camera(0, speed, deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)
        move_camera(-speed, 0, deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)
        move_camera(speed, 0, deltaTime);
    }

    mouse_set_zoom(wanted_position, window, utils::lerp(wanted_zoom, zoom, pow(zoom_speed, 60*deltaTime)));
}

void Camera::mouse_smooth_zoom_set(sf::Vector2i pixel, float level) {
    wanted_position = pixel;
    wanted_zoom = level;
}


void Camera::mouse_set_zoom(sf::Vector2i pixel, const sf::RenderWindow& window, float level) {
    zoom = level;
    const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel, view) };
    set_zoom(level);
    const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel, view) };
    const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
    view.move(offsetCoords);
}

void Camera::set_zoom(float level) {
    zoom = level;
    view.setSize(windowSize / zoom);
}

void Camera::move_camera(float x, float y, double deltaTime) {
    view.move(sf::Vector2f(x / zoom * deltaTime, y / zoom * deltaTime));
}

