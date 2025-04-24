#include "view.h"


Camera::Camera(float zoom) : zoom(zoom){
};

sf::View Camera::GetView(sf::Vector2u windowSize) {
    sf::View view(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(windowSize.x, windowSize.y)));
    view.setCenter(sf::Vector2(windowSize.x / 2.f, windowSize.y / 2.f));
    view.zoom(zoom);
    return view;
}
