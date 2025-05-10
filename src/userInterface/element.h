#pragma once
#include <SFML/Graphics/Rect.hpp>
#include "SFML/Graphics/RenderWindow.hpp"
#include <iostream>

using namespace std;

class Element : public sf::FloatRect {
public:

    using sf::FloatRect::FloatRect;
    sf::Font font;
    sf::Text text;
    sf::Text tooltip_text;
    sf::RectangleShape tooltip_box;
    bool toolip_shown;
    string text_string;
    string tooltip;

    int corner_r = 4;
    int text_size = 16;
    sf::RectangleShape rect_1;
    sf::RectangleShape rect_2;
    sf::CircleShape circle_1;
    sf::CircleShape circle_2;
    sf::CircleShape circle_3;
    sf::CircleShape circle_4;

    sf::Color textColor = sf::Color(255,255,255);
    sf::Color buttonColor = sf::Color(60, 60, 60);
    sf::Color tooltipColor = sf::Color(30, 30, 30);
    sf::Color tooltipOutlineColor = sf::Color(80, 80, 80);

    Element(sf::Vector2f pos, sf::Vector2f size) : font("hih.ttf"), text(font), tooltip_text(font) {
        this->position = pos;
        this->size = size;
        normal();
        update_shapes();
    };

    void update_shapes() {
        rect_1.setSize({size.x - 2 * corner_r, size.y});
        rect_1.setPosition({position.x + corner_r, position.y});
        rect_2.setSize({size.x, size.y - 2 * corner_r});
        rect_2.setPosition({position.x, position.y + corner_r});
        circle_1.setRadius(corner_r);
        circle_1.setPosition({position.x, position.y});
        circle_2.setRadius(corner_r);
        circle_2.setPosition({position.x + size.x - corner_r * 2, position.y});
        circle_3.setRadius(corner_r);
        circle_3.setPosition({position.x, position.y + size.y - corner_r * 2});
        circle_4.setRadius(corner_r);
        circle_4.setPosition({position.x + size.x - corner_r * 2, position.y + size.y - corner_r * 2});

        text.setCharacterSize(text_size);
        text.setString("LI");
        text.setString(text_string);
        text.setPosition({position.x - 1 + (size.x - text.getLocalBounds().size.x)/2, position.y - 1 + (size.y - text.getLocalBounds().size.y)/2});

        tooltip_text.setCharacterSize(16);
        tooltip_text.setString(tooltip);

        tooltip_box.setSize(tooltip_text.getLocalBounds().size + sf::Vector2f(8, 8));
        tooltip_box.setFillColor(tooltipColor);
        tooltip_box.setOutlineColor(tooltipOutlineColor);
        tooltip_box.setOutlineThickness(1);
    }

    void draw(sf::RenderWindow* window) {
        rect_1.setFillColor(rect_1.getFillColor());
        rect_2.setFillColor(rect_1.getFillColor());
        circle_1.setFillColor(rect_1.getFillColor());
        circle_2.setFillColor(rect_1.getFillColor());
        circle_3.setFillColor(rect_1.getFillColor());
        circle_4.setFillColor(rect_1.getFillColor());

        window->draw(rect_1);
        window->draw(rect_2);
        window->draw(circle_1);
        window->draw(circle_2);
        window->draw(circle_3);
        window->draw(circle_4);

        window->draw(text);

        if (toolip_shown) {
            sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition(*window));
            tooltip_text.setPosition(mouse_pos + sf::Vector2f(16, 0));
            tooltip_box.setPosition(tooltip_text.getGlobalBounds().position - sf::Vector2f(4, 4));

            window->draw(tooltip_box);
            window->draw(tooltip_text);
        }
    }

    virtual void normal() {
        toolip_shown = false;
        rect_1.setFillColor(buttonColor);
    }

    virtual void hover() {
        if (!tooltip.empty()) {
            toolip_shown = true;
        }
        rect_1.setFillColor(buttonColor + sf::Color(40, 40, 40));
    }

    virtual void press() {
        rect_1.setFillColor(buttonColor + sf::Color(80,80 ,80));
    }

    virtual void click_left() {
        cout << "LMB" << endl;
    };

    virtual void click_right() {
        cout << "RMB" << endl;
    }

    virtual void scroll_down() {
        cout << "UP" << endl;
    }

    virtual void scroll_up() {
        cout << "DOWN" << endl;
    }
};
