#pragma once
#include <functional>

#include "Element.h"

class Button : public Element {
public:
    Button(sf::Vector2f pos, sf::Vector2f size, std::string text, function<void()> func = nullptr)
        : Element(pos, size, func)
    {
        text_string = text;
        Element::update_shapes();
    }

    void click_left() override {
        value = 1;
        run_function();
    }
};