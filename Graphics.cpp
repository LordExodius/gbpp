// Graphics.cpp
#include "Graphics.h"
#include <SFML/Graphics.hpp>
Graphics::Graphics() : window(sf::VideoMode(200, 200), "SFML works!"), shape(100.f)
{
    shape.setFillColor(sf::Color::Green);
}

void Graphics::run()
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}
