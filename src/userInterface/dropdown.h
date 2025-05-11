#pragma once
#include <functional>

#include "Element.h"

class Dropdown : public Element {
public:
    vector<string> values;
    bool wrapping;


    Dropdown(sf::Vector2f pos, sf::Vector2f size, vector<string> values, function<void()> func = nullptr)
        : Element(pos, size, func), values(values) {
        Dropdown::update_shapes();
    }

    void update_shapes() override {
        text_string = values[value];
        Element::update_shapes();
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