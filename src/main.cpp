#include <SFML/Graphics.hpp>

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        const sf::Font font("hih.ttf");
        sf::Text text(font, "Hello SFML test", 50);
        text.setFillColor(sf::Color(25,255,255));

        window.clear();
        window.draw(text);
        window.display();
    }
}
