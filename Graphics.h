#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>
#include "MMU.h"
#include "CPU.h"

class Graphics {
    public:
        Graphics(MMU* mmu, CPU* cpu);
        ~Graphics();
        void run();
        void updateGraphics();
        void updateDisplay();
        bool isOpen();
        void updateArray(int cycles);

    private:
        int spriteSize;
        int cycleCounter;
        int scanLineCounter;
        u8 scrollX;
        u8 scrollY;
        u8 windowX;
        u8 windowY;
        bool windowEnabled;
        bool isUnsignedByte;
        u16 startAddress;
        u16 backgroundMemory;
        const int SCREEN_WIDTH = 160;
        const int SCREEN_HEIGHT = 144;
        sf::RenderWindow window;
        sf::Texture texture;
        sf::Sprite sprite;
        CPU* cpu;
        MMU* mmu;
        sf::Uint8 getPixelColor(u8 pixelValue);
        std::vector<sf::Uint8> updateScanline();
        void setInitialDisplay();
        void renderTiles();
        void renderSprites();
};

#endif
