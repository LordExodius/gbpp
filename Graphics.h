#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>
#include "Cartridge.h"
#include "CPU.h"

class Graphics {
public:
    Graphics(CPU* cpu, Cartridge* cartridge);
    ~Graphics();
    void run();
    void updateGraphics();
    bool isOpen();

    private:
        const int SCREEN_WIDTH = 160;
        const int SCREEN_HEIGHT = 144;
        const int START_ADDRESS = 0x8000;
        const int TILE_MEMORY_SIZE = 8;
        const int TILE_HEIGHT = 8;
        const int TILES_PER_ROW = SCREEN_WIDTH / TILE_HEIGHT;
        const int TILES_PER_COL = SCREEN_HEIGHT / TILE_HEIGHT;
        sf::RenderWindow window;
        sf::Texture texture;
        sf::Sprite sprite;
        CPU* cpu;
        Cartridge* cartridge;
        sf::Uint8 getPixelColor(u8 pixelValue);
};

#endif
