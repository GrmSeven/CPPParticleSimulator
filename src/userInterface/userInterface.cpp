#include "userInterface.h"

#include <iostream>
#include <cmath>

#include "checkbox.h"
#include "range.h"
#include "dropdown.h"
#include "button.h"

using namespace std;

UserInterface::UserInterface(sf::Vector2f windowSize) : font("hih.ttf"), lines(sf::PrimitiveType::Lines, 2*1) {
    view.setSize(windowSize);
    view.setCenter(windowSize / 2.f);
    create_elements();
}

void UserInterface::create_elements() {
    sf::Text text_1(font, "Particles", 14);
    text_1.setPosition({5, 5});
    text_1.setStyle(sf::Text::Bold);
    details.push_back(text_1);

    sf::Text text_2(font, "Particle count", 12);
    text_2.setPosition({5, 25});
    details.push_back(text_2);
    elements["particle_count"] = new Range({85, 23}, {62, 19}, false, true, 1000, 100, 0, 1000000);

    sf::Text text_3(font, "Particle types", 12);
    text_3.setPosition({5, 45});
    details.push_back(text_3);
    elements["particle_types"] = new Range({85, 43}, {62, 19}, false, true, 8, 1, 1, 100);

    // Matrix

    // Randomize button, reset button
    // OR
    // Action: Dropdown with (Randomize, Reset, ) and button Apply

    add_line(0, 265);

    // sf::Text text_4(font, "Mouse", 14);
    // text_4.setPosition({5, 70});
    // text_4.setStyle(sf::Text::Bold);
    // details.push_back(text_4);
    //
    // sf::Text text_5(font, "Drag type:", 12);
    // text_5.setPosition({5, 90});
    // details.push_back(text_5);
    //
    // sf::Text text_6(font, "Drag force:", 12);
    // text_6.setPosition({5, 110});
    // details.push_back(text_6);

    // sf::Text text_7(font, "Mode:", 12);
    // text_7.setPosition({5, 10});
    // details.push_back(text_7);
    //
    // sf::Text text_8(font, "Spawn count:", 12);
    // text_8.setPosition({5, 10});
    // details.push_back(text_8);
    //
    // sf::Text text_9(font, "Spawn type:", 12);
    // text_9.setPosition({5, 10});
    // details.push_back(text_9);
    //
    // sf::Text text_10(font, "Physics", 14);
    // text_10.setPosition({5, 10});
    // text_10.setStyle(sf::Text::Bold);
    // details.push_back(text_10);
    //
    // sf::Text text_11(font, "Wrapping", 12);
    // text_11.setPosition({5, 10});
    // details.push_back(text_11);
    //
    // sf::Text text_12(font, "Behaviour formula:", 12);
    // text_12.setPosition({5, 10});
    // details.push_back(text_12);
    //
    // sf::Text text_13(font, "Min distance:", 12);
    // text_13.setPosition({5, 10});
    // details.push_back(text_13);
    //
    // sf::Text text_14(font, "Interation radius:", 12);
    // text_14.setPosition({5, 10});
    // details.push_back(text_14);
    //
    // sf::Text text_15(font, "Force multiplier:", 12);
    // text_15.setPosition({5, 10});
    // details.push_back(text_15);
    //
    // sf::Text text_16(font, "Use terminal velocity", 12);
    // text_16.setPosition({5, 10});
    // details.push_back(text_16);
    //
    // sf::Text text_17(font, "Terminal velocity:", 12);
    // text_17.setPosition({5, 10});
    // details.push_back(text_17);
    //
    // sf::Text text_18(font, "Rendering", 14);
    // text_18.setPosition({5, 10});
    // text_18.setStyle(sf::Text::Bold);
    // details.push_back(text_18);
    //
    // sf::Text text_19(font, "Particle vertex count:", 12);
    // text_19.setPosition({5, 10});
    // details.push_back(text_19);
    //
    // sf::Text text_20(font, "Particle radius:", 12);
    // text_20.setPosition({5, 10});
    // details.push_back(text_20);
    //
    // sf::Text text_21(font, "Palette:", 12);
    // text_21.setPosition({5, 10});
    // details.push_back(text_21);


    for (auto& element : elements) {
        element.second->update_shapes();
    }
}

void UserInterface::add_line(int i, int x) {
    lines[i*2 + 0].position = sf::Vector2f(0, x + 0.5);
    lines[i*2 + 1].position = sf::Vector2f(sidebar_size, x + 0.5);
    lines[i*2 + 0].color = line_color;
    lines[i*2 + 1].color = line_color;
}

void UserInterface::render(sf::RenderWindow& window) {
    window.setView(view);

    // Sidebar
    sf::RectangleShape sidebar;
    sidebar.setSize({sidebar_size, view.getSize().y});
    sidebar.setFillColor(background_color);
    sidebar.setOutlineColor(line_color);
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

    // Text
    for (auto &t : details) {
        window.draw(t);
    }

    // Lines
    window.draw(lines);

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
