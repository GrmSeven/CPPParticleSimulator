#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Mouse.hpp"

class UI {
public:
    void handle_events(std::optional<sf::Event>& event);
    void render();
};



