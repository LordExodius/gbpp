// Graphics.cpp
#include "Graphics.h"

Graphics::Graphics(CPU* cpu, Cartridge* cartridge) : window(sf::VideoMode(160, 144), "Gameboy Emulator") {
    this->cpu = cpu;
    this->cartridge = cartridge;
    texture.create(SCREEN_WIDTH, SCREEN_HEIGHT);
    sprite.setTexture(texture);
    run();
}

void Graphics::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        updateGraphics();
        window.display();
    }
}

void Graphics::updateGraphics() {
    sf::Uint8 pixels[SCREEN_WIDTH * SCREEN_HEIGHT * 4];

    // Tile calculation
    for (int tileRow = 0; tileRow < TILES_PER_COL; tileRow++) {
        for (int tileCol = 0; tileCol < TILES_PER_ROW; tileCol++) {
            // Rewrite this line to add every iteration instead of recalculating
            int tileAddress = START_ADDRESS + (tileRow * TILES_PER_ROW + tileCol) * TILE_MEMORY_SIZE;
            printf("%d\n", tileAddress);

            // Pixel calculation
            for (int pixelRow = 0; pixelRow < TILE_HEIGHT; pixelRow++) {
                for (int pixelCol = 0; pixelCol < TILE_HEIGHT; pixelCol++) {
                    u8 pixelValue = cartridge->getMemory(tileAddress + pixelRow * 2) >> (7 - pixelCol % TILE_HEIGHT) & 0x01;
                    int pixelIndex = ((tileRow * TILE_HEIGHT + pixelRow) * SCREEN_WIDTH + tileCol * TILE_HEIGHT + pixelCol) * 4;
                    
                    pixels[pixelIndex] = getPixelColor(pixelValue);      // Red channel
                    pixels[pixelIndex + 1] = getPixelColor(pixelValue);  // Green channel
                    pixels[pixelIndex + 2] = getPixelColor(pixelValue);  // Blue channel
                    pixels[pixelIndex + 3] = 255;
                }
            }
        }
    }

    texture.update(pixels);
    window.draw(sprite);
}

sf::Uint8 Graphics::getPixelColor(u8 pixelValue) {
    switch (pixelValue) {
        case 0:
            return 255; // White
        case 1:
            return 192; // Light grey
        case 2:
            return 128; // Medium grey
        case 3:
            return 64;  // Dark grey
        default:
            return 0;   //  Black
    }
}

bool Graphics::isOpen() {
    return window.isOpen();
}

Graphics::~Graphics() {
}
