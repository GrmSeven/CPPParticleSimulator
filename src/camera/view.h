#ifndef VIEW_H
#define VIEW_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"

class Camera {
public:
    Camera(float zoom = 1.f);

    sf::View GetView(sf::Vector2u windowSize);
    void update(double deltaTime);
    void change_zoom(float step);
    void move_camera(float x, float y, double deltaTime);
    void zoomViewAt(sf::View& view, sf::Vector2i pixel, const sf::RenderWindow& window, float zoom);

    float zoom;
    sf::Vector2f position;
    float speed = 1000.f;
    float zoom_speed = 1.1f;
};

#endif // VIEW_H
