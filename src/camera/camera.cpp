#include "camera.h"
#include "../utils.h"
#include <cmath>
#include <iostream>
#include <ostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Mouse.hpp"

Camera::Camera(float zoom, sf::Vector2f simulationSize, sf::Vector2f windowSize) : windowSize(windowSize), zoom(zoom), wanted_zoom(zoom) {
    view.setSize(windowSize * zoom);
    view.setCenter(simulationSize / 2.f);
}

void Camera::handle_events(std::optional<sf::Event>& event) {
    if (is_active) {
        // Shift detection
        if (const auto* keyRelased = event->getIf<sf::Event::KeyPressed>()) {
            if (keyRelased->code == sf::Keyboard::Key::LShift) {
                shift_pressed = true;
            }
        }

        // Zooming
        if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
            if (!shift_pressed) {
                float scroll_delta = mouseWheelScrolled->delta;
                if (scroll_delta > 0) {
                    // mouse_smooth_zoom_set({ mouseWheelScrolled->position }, zoom*1.1);
                    mouse_smooth_zoom_set({ mouseWheelScrolled->position }, wanted_zoom/zoom_sensitivity);
                } else {
                    // mouse_smooth_zoom_set({ mouseWheelScrolled->position }, zoom/1.1);
                    mouse_smooth_zoom_set({ mouseWheelScrolled->position }, wanted_zoom*zoom_sensitivity);
                }
            }
        }

        // Dragging
        if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButtonPressed->button == sf::Mouse::Button::Middle)
            {
                prev_mouse_pos = {mouseButtonPressed->position.x, mouseButtonPressed->position.y};
                is_dragging = true;
            }
        }
    }

    // Shift detection
    if (const auto* keyRelased = event->getIf<sf::Event::KeyReleased>()) {
        if (keyRelased->code == sf::Keyboard::Key::LShift) {
            shift_pressed = false;
        }
    }

    // Dragging
    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Middle)
        {
            is_dragging = false;
        }
    }
}

void Camera::update(sf::RenderWindow& window, double deltaTime) {
    if (is_dragging) {
        auto mouse_pos = sf::Mouse::getPosition(window);
        view.move(sf::Vector2f((prev_mouse_pos.x - mouse_pos.x)*zoom, (prev_mouse_pos.y - mouse_pos.y)*zoom));
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
    clamp_camera();
}

void Camera::move_camera(float x, float y, double deltaTime) {
    view.move(sf::Vector2f(x * zoom * deltaTime, y * zoom * deltaTime));
}

/*
 * Doesn't let camera outside of bounds
 */
void Camera::clamp_camera() {
    view.move(utils::clamp(view.getCenter(), sf::Vector2f(-2003, -2003), sf::Vector2f(2003*2, 2003*2)) - view.getCenter());
}

void Camera::mouse_smooth_zoom_set(sf::Vector2i pixel, float level) {
    wanted_position = pixel;
    wanted_zoom = utils::clamp(level, min_zoom, max_zoom);
}

void Camera::mouse_set_zoom(sf::Vector2i pixel, const sf::RenderWindow& window, float level) {
    const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel, view) };
    view.setSize(windowSize * level);
    const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel, view) };
    const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
    view.move(offsetCoords);
    zoom = level;
}

void Camera::resize_window(sf::Vector2f newWindosSize) {
    float zoom_mult = windowSize.y / newWindosSize.y;
    zoom *= zoom_mult;
    wanted_zoom *= zoom_mult;
    windowSize = newWindosSize;
    view.setSize(windowSize * zoom);

}
