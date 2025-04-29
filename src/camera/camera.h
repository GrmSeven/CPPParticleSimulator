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
    void update(sf::RenderWindow& window, double deltaTime);
    void move_camera(float x, float y, double deltaTime);
    void mouse_zoom(sf::View& view, sf::Vector2i pixel, const sf::RenderWindow& window, float zoom);

    sf::Vector2i prev_mouse_pos;
    bool is_dragging;

    sf::Vector2f windowSize;
    float speed = 1000.f;
    float zoom_speed = 1.1f;
    float getViewZoom();
};

#endif // VIEW_H
