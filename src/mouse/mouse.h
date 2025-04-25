#ifndef MOUSE_H
#define MOUSE_H

#include "SFML/System/Vector2.hpp"
#include "../particles/particle.h"

class CMouse {
public:
    sf::Vector2i mousePos;
    particle* cursorParticle;

    CMouse(sf::Vector2i mouse, particle* cursor);
};

#endif
