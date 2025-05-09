#pragma once
#include <SFML/Graphics.hpp>
#include "vector"
#include "element.h"

class UserInterface {
public:
    sf::View view;
    float fps_counter;
    float sidebar_size = 200.f;

    sf::Vector2i first_press_pos;
    bool is_mouse_held{};

    std::vector<Element*> elements;

    UserInterface(sf::Vector2f windowSize);
    void render(sf::RenderWindow& window);

    void create_elements();

    template <typename T>
    bool is_element_touching(Element* element, sf::Vector2<T> pos);
    Element* get_element_at(sf::Vector2i pos);
    void mouse_moved(sf::Vector2i pos);
    void mouse_pressed(sf::Vector2i pos, bool is_left);
    void mouse_released(sf::Vector2i pos, bool is_left);
    void mouse_scrolled(sf::Vector2i pos, float scroll_delta);

    void resize(sf::Vector2f newWindowSize);
};

