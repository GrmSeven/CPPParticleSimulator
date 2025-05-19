#pragma once
#include "element.h"

class Range : public Element {
public:
    float interval;
    float default_value;


    Range(sf::Vector2f pos, sf::Vector2f size, float default_value, float interval, float min_value, float max_value, function<void()> func = nullptr)
        : Element(pos, size, func), interval(interval), default_value(default_value)
    {
        this->max_value = max_value;
        this->min_value = min_value;
        value = default_value;
        Range::update_shapes();
    }

    void update_shapes() override {
        value = utils::clamp(value, min_value, max_value);
        text_string = utils::float_to_string(value, 2);
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