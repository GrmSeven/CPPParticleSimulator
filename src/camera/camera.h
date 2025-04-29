#ifndef VIEW_H
#define VIEW_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"

class Camera {
public:
    Camera(float zoom,sf::Vector2f position, sf::Vector2f windowSize);
    sf::View view;
    sf::Vector2f windowSize;
    float zoom;
    float speed = 1000.f;

    void update(sf::RenderWindow& window, double deltaTime);
    void move_camera(float x, float y, double deltaTime);
    void mouse_smooth_zoom_set(sf::Vector2i pixel, float level);
    void mouse_smooth_zoom_update(sf::Vector2i pixel, const sf::RenderWindow& window, float level);
    void mouse_set_zoom(sf::Vector2i pixel, const sf::RenderWindow& window, float level);
    void set_zoom(float level);

    // Dragging
    sf::Vector2i prev_mouse_pos;
    bool is_dragging{};
    // Smooth zooming
    sf::Vector2i wanted_position;
    float wanted_zoom;
    float zoom_speed = 0.8f;
    float zoom_sensitivity = 1.1f;
};

#endif // VIEW_H
