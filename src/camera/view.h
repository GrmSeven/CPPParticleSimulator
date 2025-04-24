#ifndef VIEW_H
#define VIEW_H
#include "SFML/Graphics/View.hpp"
#include "SFML/System/Vector2.hpp"


class Camera {
    public:
    Camera(float zoom = 1.f);
    sf::View GetView(sf::Vector2u windowSize);

    float zoom;
};


#endif //VIEW_H
