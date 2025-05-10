#pragma once
#include <functional>

#include "Element.h"

class Dropdown : public Element {
public:
    int value;
    vector<string> values;


    Dropdown(sf::Vector2f pos, sf::Vector2f size, vector<string> values)
        : Element(pos, size), value(0), values(values) {
        update_shapes();
    }

    void update_shapes() override {
        text_string = values[value];
        Element::update_shapes();
    }

    void click_left() override {
        value = value + 1;
        if (value > static_cast<int>(values.size())-1) value = 0;
        update_shapes();
    }

    void click_right() override {
        value = value - 1;
        if (value < 0) value = static_cast<int>(values.size())-1;
        update_shapes();
    }

    void scroll_down() override {
        click_right();
    }

    void scroll_up() override {
        click_left();
    }
};