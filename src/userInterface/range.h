#pragma once
#include "Element.h"

class Range : public Element {
public:
    float interval;
    float min_value;
    float max_value;
    float default_value;

    Range(sf::Vector2f pos, sf::Vector2f size, float default_value, float interval, float min_value, float max_value, function<void()> func = nullptr)
        : Element(pos, size, func), interval(interval), min_value(min_value), max_value(max_value), default_value(default_value)
    {
        value = default_value;
        Range::update_shapes();
    }

    void update_shapes() override {
        text_string = to_string(value);
        while (!text_string.empty() && text_string.back() == '0') {
            text_string.pop_back();
        }
        if (text_string.back() == '.') {
            text_string.pop_back();
        }
        Element::update_shapes();
    }

    void click_middle() override {
        value = default_value;
        run_function();
        update_shapes();
    }

    void click_left() override {
        value = min(max_value, value + interval);
        run_function();
        update_shapes();
    }

    void click_right() override {
        value = max(min_value, value - interval);
        run_function();
        update_shapes();
    }

    void scroll_down() override {
        click_left();
    }

    void scroll_up() override {
        click_right();
    }
};