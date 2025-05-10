#pragma once
#include "Element.h"

class Checkbox : public Element {
public:
    bool value;
    std::string label;

    Checkbox(sf::Vector2f pos, sf::Vector2f size, std::string text, bool default_value = false)
        : Element(pos, size), value(default_value), label(text) {
        update_text();
    }

    void update_text() {
        text_string = value ? "[ v ]" + label : "[    ]" + label;
        this->text.setString(text_string);
        update_shapes();
    }

    void click_left() override {
        value = !value;
        update_text();
    }
};