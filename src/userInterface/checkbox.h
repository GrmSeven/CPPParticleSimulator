#pragma once
#include "Element.h"

class Checkbox : public Element {
public:
    bool value;
    std::string label;
    sf::VertexArray checkmark;

    Checkbox(sf::Vector2f pos, sf::Vector2f size, std::string text, bool default_value = false, function<void()> func = nullptr)
        : Element(pos, size, func), value(default_value), label(text),
        checkmark(sf::PrimitiveType::TriangleFan, 6)
    {
        update_shapes();
    }

    void update_shapes() override {
        text_string = value ? "" + label : "" + label;

        sf::Vector2f shift = {position.x - 7 + size.y/2, position.y - 6 + size.y/2};
        checkmark[0].position = {5 + shift.x, 7 + shift.y};
        checkmark[1].position = {12 + shift.x, 0 + shift.y};
        checkmark[2].position = {14 + shift.x, 2 + shift.y};
        checkmark[3].position = {5 + shift.x, 11 + shift.y};
        checkmark[4].position = {0 + shift.x, 6 + shift.y};
        checkmark[5].position = {2 + shift.x, 4 + shift.y};
        for (int i = 0; i < 6; i++) {
            if (value) {
                checkmark[i].color = sf::Color::White;
            } else {
                checkmark[i].color = sf::Color::Transparent;
            }
        }
        Element::update_shapes();
    }

    void draw(sf::RenderWindow* window) override {
        Element::draw(window);
        window->draw(checkmark);
    }

    void click_left() override {
        value = !value;
        update_shapes();
        run_function();
    }
};