#pragma once
#include "Element.h"

class Checkbox : public Element {
public:
    std::string label;
    sf::VertexArray checkmark;
    bool default_value;

    Checkbox(sf::Vector2f pos, sf::Vector2f size, std::string text, bool default_value = false, function<void()> func = nullptr)
        : Element(pos, size, func), label(text), default_value(default_value),
        checkmark(sf::PrimitiveType::TriangleFan, 6)
    {
        value = default_value;
        Checkbox::update_shapes();
    }

    void update_shapes() override {
        Element::update_shapes();

        text_string = value ? "" + label : "" + label;

        sf::Vector2f shift = {position.x - 6 + size.y/2, position.y - 5 + size.y/2};

        checkmark[0].position = {4 + shift.x, 6 + shift.y};
        checkmark[1].position = {10 + shift.x, 0 + shift.y};
        checkmark[2].position = {11.5f + shift.x, 1.5f + shift.y};
        checkmark[3].position = {4 + shift.x, 9 + shift.y};
        checkmark[4].position = {0 + shift.x, 5 + shift.y};
        checkmark[5].position = {1.5f + shift.x, 3.5f + shift.y};

        for (int i = 0; i < 6; i++) {
            if (value) {
                checkmark[i].color = currentTextColor;
            } else {
                checkmark[i].color = sf::Color::Transparent;
            }
        }
    }

    void draw(sf::RenderWindow* window) override {
        Element::draw(window);
        window->draw(checkmark);
    }

    void click_middle() override {
        value = default_value;
        run_function();
        update_shapes();
    }

    void click_left() override {
        value = !value;
        run_function();
        update_shapes();
    }
};