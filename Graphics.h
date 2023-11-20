#pragma once

#include <SFML/Graphics.hpp>

class Graphics
{
public:
    Graphics();
    void run();

private:
    sf::RenderWindow window;
    sf::CircleShape shape;
};