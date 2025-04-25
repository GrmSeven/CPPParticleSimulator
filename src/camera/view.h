#ifndef VIEW_H
#define VIEW_H

#include "SFML/Graphics/View.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"

class Camera {
public:
    Camera(float zoom = 1.f);
    sf::View GetView(sf::Vector2u windowSize);
    void update(double deltaTime);

    float zoom;
    sf::Vector2f position;
    float speed = 10.f;
    float zoom_speed = 1.01f;
};

#endif // VIEW_H
