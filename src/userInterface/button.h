#pragma once
#include <functional>

#include "Element.h"

class Button : public Element {
public:
    function<void()> func;

    Button(sf::Vector2f pos, sf::Vector2f size, std::string text, function<void()> func)
        : Element(pos, size), func(func)
    {
        text_string = text;
        update_shapes();
    }

    void click_left() override {
        func();
    }
};