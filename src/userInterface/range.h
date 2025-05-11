#pragma once
#include "Element.h"

class Range : public Element {
public:
    float value;
    bool display_color;
    bool display_value;
    float interval;
    float min_value;
    float max_value;

    Range(sf::Vector2f pos, sf::Vector2f size, bool display_color, bool display_value, float default_value, float interval, float min_value, float max_value, function<void()> func = nullptr)
        : Element(pos, size, func), value(default_value),
    display_color(display_color), display_value(display_value), interval(interval), min_value(min_value), max_value(max_value)
    {
        text_centered = true;
        update_shapes();

    }

    void update_shapes() override {
        if (display_value) {
            text_string = to_string(value);
            while (!text_string.empty() && text_string.back() == '0') {
                text_string.pop_back();
            }
            if (text_string.back() == '.') {
                text_string.pop_back();
            }
        } else {
            text_string = "";
        }

        if (display_color) {
            float c = value / max_value * (255-currentButtonColor.r);
            rect_1.setFillColor(currentButtonColor + sf::Color(max(-c, 0.f), 0, max(c, 0.f)));
            if (disabled) {
                rect_1.setFillColor(disabledColor + sf::Color(max(-c, 0.f), 0, max(c, 0.f)));
            }
        }

        Element::update_shapes();
    }

    void normal() override {
        toolip_shown = false;
        if (display_color) {
            float c = value / max_value * (255-currentButtonColor.r);
            rect_1.setFillColor(currentButtonColor + sf::Color(max(-c, 0.f), 0, max(c, 0.f)));
            if (disabled) {
                rect_1.setFillColor(disabledColor + sf::Color(max(-c, 0.f), 0, max(c, 0.f)));
            }
        }
        Element::normal();
    }

    void hover() override {
        if (!tooltip.empty()) {
            toolip_shown = true;
        }
        if (display_color) {
            float c = value / max_value * (255-currentButtonColor.r);
            rect_1.setFillColor(currentButtonColor + sf::Color(40, 40, 40) + sf::Color(max(-c, 0.f), 0, max(c, 0.f)));
            if (disabled) {
                rect_1.setFillColor(disabledColor + sf::Color(max(-c, 0.f), 0, max(c, 0.f)));
            }
        }
        Element::hover();
    }

    void press() override {
        if (display_color) {
            float c = value / max_value * (255-currentButtonColor.r);
            rect_1.setFillColor(currentButtonColor + sf::Color(80,80 ,80) + sf::Color(max(-c, 0.f), 0, max(c, 0.f)));
            if (disabled) {
                rect_1.setFillColor(disabledColor + sf::Color(max(-c, 0.f), 0, max(c, 0.f)));
            }
        }
        run_function();
        Element::press();
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