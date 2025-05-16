#pragma once
#include <functional>

#include "element.h"

class Dropdown : public Element {
public:
    vector<string> values;
    bool wrapping;
    sf::VertexArray arrows;


    Dropdown(sf::Vector2f pos, sf::Vector2f size, vector<string> values, function<void()> func = nullptr)
        : Element(pos, size, func), values(values),
        arrows(sf::PrimitiveType::Triangles, 6) {
        value = 0;
        Dropdown::update_shapes();
    }

    void update_shapes() override {
        text_string = values[value];
        Element::update_shapes();

        sf::Vector2f shift = {position.x - 5 + size.y/2, position.y - 6 + size.y/2};

        arrows[0].position = {0 + shift.x, 5 + shift.y};
        arrows[1].position = {5.5f + shift.x, 0 + shift.y};
        arrows[2].position = {11 + shift.x, 5 + shift.y};
        arrows[3].position = {0 + shift.x, 7 + shift.y};
        arrows[4].position = {11 + shift.x, 7 + shift.y};
        arrows[5].position = {5.5f + shift.x, 12 + shift.y};

        for (int i = 0; i < 6; i++) {
            arrows[i].color = currentTextColor;
        }
    }

    void draw(sf::RenderWindow* window) override {
        Element::draw(window);
        window->draw(arrows);
    }

    void click_middle() override {
        value = 0;
        run_function();
        update_shapes();
    }

    void click_left() override {
        value = value + 1;
        if (wrapping) {
            if (value > static_cast<int>(values.size())-1) value = 0;
        } else {
            value = min(static_cast<int>(value), static_cast<int>(values.size())-1);
        }
        update_shapes();
        run_function();
    }

    void click_right() override {
        value = value - 1;
        if (wrapping) {
            if (value < 0) value = static_cast<int>(values.size())-1;
        } else {
            value = max(static_cast<int>(value), 0);
        }
        update_shapes();
        run_function();
    }

    void scroll_down() override {
        click_right();
    }

    void scroll_up() override {
        click_left();
    }
};