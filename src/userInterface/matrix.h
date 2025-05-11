#pragma once
#include "Element.h"
#include <cmath>

class Matrix : public Element {
public:
    float interval;
    float min_value;
    float max_value;
    size_t matrix_size;
    int used_palette;
    std::vector<std::vector<float>> particle_interaction_matrix = {
        {1, 0, 0, 0, 0, 0, 0, 0.5},
        {0.5, 1, 0, 0, 0, 0, 0, 0},
        {0, 0.5, 1, 0, 0, 0, 0, 0},
        {0, 0, 0.5, 1, 0, 0, 0, 0},
        {0, 0, 0, 0.5, 1, 0, 0, 0},
        {0, 0, 0, 0, 0.5, 1, 0, 0},
        {0, 0, 0, 0, 0, 0.5, 1, 0},
        {0, 0, 0, 0, 0, 0, 0.5, 1}
    };
    std::vector<sf::RectangleShape> cells;
    std::vector<sf::CircleShape> circles;

    Matrix(sf::Vector2f pos, sf::Vector2f size, int default_matrix_size, float default_value, float interval, float min_value, float max_value, function<void()> func = nullptr)
        : Element(pos, size, func), interval(interval), min_value(min_value), max_value(max_value), matrix_size(default_matrix_size) {
        resize_matrix(matrix_size);
        currentButtonColor = sf::Color::Transparent;
        Matrix::update_shapes();
    }

    void resize_matrix(size_t s) {
        particle_interaction_matrix.resize(s, std::vector<float>(s, 0));
        for (size_t i = 0; i < s; i++) {
            particle_interaction_matrix[i].resize(s, 0);
        }
    }

    void update_shapes() override {
        cells.clear();
        cells.reserve(matrix_size * matrix_size);
        float s = (size.x)/(matrix_size+1);
        for (int i = 0; i < matrix_size; i++) {
            for (int j = 0; j < matrix_size; j++) {
                sf::RectangleShape cell({s, s});
                cell.setPosition({(j+1)*s+position.x, (i+1)*s+position.y});
                float value = particle_interaction_matrix[i][j];
                cell.setFillColor(sf::Color(max(-value, 0.f)*255, 0, max(value, 0.f)*255) + disabledColor);
                cells.emplace_back(cell);
            }
        }
        for (size_t i = 0; i < matrix_size; i++) {
            sf::CircleShape circle(s*0.5);
            circle.setPosition({position.x, (i+1)*s+position.y});
            circle.setFillColor(get_particle_color(i));
            circles.emplace_back(circle);

            sf::CircleShape circle_2(s*0.5);
            circle_2.setPosition({(i+1)*s+position.x, position.y});
            circle_2.setFillColor(get_particle_color(i));
            circles.emplace_back(circle_2);
        }
    }

    sf::Color get_particle_color(size_t type) {
        switch (used_palette) {
            case 0:
                return get_palette_color(fmod(type/static_cast<float>(matrix_size), 1.f), {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}, {0.00, 0.67, 0.33});
        }
        return sf::Color::White;
    }

    sf::Color get_palette_color(float t, sf::Vector3f a, sf::Vector3f b, sf::Vector3f c, sf::Vector3f d) {
        return sf::Color((a.x + b.x*cos( 2*M_PI*(c.x*t + d.x)))*255.f, (a.y + b.y*cos( 2*M_PI*(c.y*t + d.y)))*255.f, (a.z + b.z*cos( 2*M_PI*(c.z*t + d.z)))*255.f);
    }


    virtual void draw(sf::RenderWindow* window) {
        for (auto& cell : cells) {
            window->draw(cell);
        }
        for (auto& circle : circles) {
            window->draw(circle);
        }
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