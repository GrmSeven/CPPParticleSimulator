#include "userInterface.h"

#include <iostream>
#include <cmath>

using namespace std;

UserInterface::UserInterface(sf::Vector2f windowSize) {
    view.setSize(windowSize);
    view.setCenter(windowSize / 2.f);
    create_elements();
}

void UserInterface::create_elements() {
    elements.push_back(new Element({0, 0}, {50, 30}));
    elements.push_back(new Element({70, 0}, {50, 30}));
}

void UserInterface::render(sf::RenderWindow& window) {
    window.setView(view);

    // Sidebar
    sf::RectangleShape sidebar;
    sidebar.setSize({sidebar_size, view.getSize().y});
    sidebar.setFillColor(sf::Color(20, 20, 20));
    window.draw(sidebar);

    // Elements (Buttons and stuff)
    for (auto& element : elements) {
        element->draw(&window);
    }

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

template <typename T>
bool UserInterface::is_element_touching(Element* element, sf::Vector2<T> pos) {
    return element->contains(static_cast<sf::Vector2f>(pos));
}

Element* UserInterface::get_element_at(sf::Vector2i pos) {
    for (auto& element : elements) {
        if (is_element_touching(element, pos)) return element;
    }
    return nullptr;
}

void UserInterface::mouse_moved(sf::Vector2i pos) {
    for (auto& element : elements) {
        if (is_element_touching(element, pos)) {
            if (is_mouse_held) {
                Element* press_element = get_element_at(first_press_pos);
                Element* release_element = get_element_at(pos);
                if (release_element != nullptr) {
                    if (press_element == release_element) {
                        release_element->press();
                    } else {
                        release_element->hover();
                    }
                }
            } else {
                element->hover();
            }
        } else {
            element->normal();
        }
    }
}

void UserInterface::mouse_pressed(sf::Vector2i pos, bool is_left) {
    first_press_pos = pos;
    is_mouse_held = true;
    Element* press_element = get_element_at(first_press_pos);
    if (press_element != nullptr) {
        press_element->press();
    }
}

void UserInterface::mouse_released(sf::Vector2i pos, bool is_left) {
    is_mouse_held = false;
    Element* press_element = get_element_at(first_press_pos);
    Element* release_element = get_element_at(pos);
    if (release_element != nullptr) {
        if (press_element == release_element) {
            release_element->hover();
            if (is_left) {
                release_element->click_left();
            } else {
                release_element->click_right();
            }
            first_press_pos = sf::Vector2i(-10000, -10000);
        } else {
            release_element->hover();
        }
    }
}

void UserInterface::mouse_scrolled(sf::Vector2i pos, float scroll_delta) {
    Element* element = get_element_at(pos);
    if (element != nullptr) {
        element->hover();
        if (scroll_delta > 0) {
            element->scroll_down();
        } else {
            element->scroll_up();
        }
    }
}

void UserInterface::resize(sf::Vector2f newWindowSize) {
    view.setSize(newWindowSize);
    view.setCenter(newWindowSize / 2.f);
}
