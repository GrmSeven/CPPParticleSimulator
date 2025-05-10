#include "userInterface.h"

#include <iostream>
#include <cmath>

#include "checkbox.h"
#include "range.h"
#include "dropdown.h"
#include "button.h"

using namespace std;

UserInterface::UserInterface(sf::Vector2f windowSize) : font("hih.ttf") {
    view.setSize(windowSize);
    view.setCenter(windowSize / 2.f);
    create_elements();
}

void UserInterface::create_elements() {
    elements["test_check"] = new Checkbox({10, 10}, {20, 20}, "", true);
    elements["test_check_2"] = new Checkbox({56, 10}, {108, 30}, "     Aaa", true);
    elements["test_check_3"] = new Range({56, 60}, {108, 30}, true, true, 0, 0.5, -1, 1);
    elements["test_check_3"]->text_centered = true;
    elements["test_check_4"] = new Dropdown({56, 110}, {108, 30}, {"One", "Two", "Three", "Four", "Five", "Six", "Seven"});
    elements["test_check_5"] = new Button({56, 160}, {108, 30}, "Button", [this]{this->elements["test_check_2"]->disable();});

    for (auto& element : elements) {
        element.second->update_shapes();
    }
}

void UserInterface::render(sf::RenderWindow& window) {
    window.setView(view);

    // Sidebar
    sf::RectangleShape sidebar;
    sidebar.setSize({sidebar_size, view.getSize().y});
    sidebar.setFillColor(sf::Color(15, 15, 15));
    sidebar.setOutlineColor(sf::Color(30, 30, 30));
    sidebar.setOutlineThickness(1);
    window.draw(sidebar);

    // Elements (Buttons and stuff)
    for (auto& element : elements) {
        if (!is_element_touching(element.second, mouse_pos)) {
            element.second->draw(&window);
        }
    }
    if (get_element_at(mouse_pos) != nullptr) {
        get_element_at(mouse_pos)->draw(&window);
    }


    // FPS counter
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
        if (is_element_touching(element.second, pos)) return element.second;
    }
    return nullptr;
}

void UserInterface::mouse_moved(sf::Vector2i pos) {
    mouse_pos = pos;
    for (auto& element : elements) {
        if (is_element_touching(element.second, pos)) {
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
                element.second->hover();
            }
        } else {
            element.second->normal();
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
