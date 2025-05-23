#pragma once
#include <SFML/Graphics.hpp>
#include "vector"
#include <map>
#include "element.h"
#include "matrix.h"

class UserInterface {
public:
    sf::View view;
    int fps_counter;
    const sf::Font font;
    float sidebar_size = 200.f;
    sf::Vector2i mouse_pos;

    sf::Color background_color = sf::Color(15, 15, 15);
    sf::Color line_color = sf::Color(40, 40, 40);

    sf::Vector2i first_press_pos;
    bool is_mouse_held{};

    vector<Element*> elements;
    Matrix* matrix;
    vector<sf::Text> details;
    sf::VertexArray lines;
    sf::RectangleShape sidebar;
    sf::Text fps_text;

    UserInterface(sf::Vector2f windowSize);
    void render(sf::RenderWindow& window);

    void add_line(int i, int x);
    void create_elements();

    template <typename T>
    bool is_element_touching(Element* element, sf::Vector2<T> pos);
    Element* get_element_at(sf::Vector2i pos);
    void mouse_moved(sf::Vector2i pos);
    void mouse_pressed(sf::Vector2i pos, int type);
    void mouse_released(sf::Vector2i pos, int type);
    void mouse_scrolled(sf::Vector2i pos, float scroll_delta, bool shift_pressed);

    void resize(sf::Vector2f newWindowSize);
};

