#include "Graphics.h"
#include <iostream>
#include <bitset>

Graphics::Graphics(MMU* mmu, CPU* cpu) : window(sf::VideoMode(160, 144), "Gameboy Emulator") {
    this->cpu = cpu;
    this->mmu = mmu;
    window.setFramerateLimit(60);
    texture.create(SCREEN_WIDTH, SCREEN_HEIGHT);
    sprite.setTexture(texture);

    setInitialDisplay();
    run();
}

void Graphics::setInitialDisplay() {
    // Read LCDC register for initial display configuration
    u8 u8lcdc = mmu->readByte(0xFF40);
    std::bitset<8> lcdc(u8lcdc);
    printf("LCDC: %d\n", lcdc);

    // Determine visual area and window
    scrollY = mmu->readByte(0xFF42);
    scrollX = mmu->readByte(0xFF43);
    windowY = mmu->readByte(0xFF4A);
    windowX = mmu->readByte(0xFF4B) - 7;

    printf("Window X: %d\n", windowX);
    printf("Window Y: %d\n", windowY);
    printf("Scroll X: %d\n", scrollX);
    printf("Scroll Y: %d\n", scrollY);

    u8 yPosition = 0;

    // Determine whether window is enabled
    windowEnabled = false;
    isUnsignedByte = true;

    if (lcdc[5] == 1) {
        if (windowY <= mmu->readByte(0xFF44) && windowX <= mmu->readByte(0xFF43)) {
            windowEnabled = true;
        }
    }

    // Background memory
    if (windowEnabled) {
        if (lcdc[3] == 0) {
            backgroundMemory = 0x9800;
        } else {
            backgroundMemory = 0x9C00;
        }
    } else {
        if (lcdc[6] == 0) {
            backgroundMemory = 0x9800;
        } else {
            backgroundMemory = 0x9C00;
        }
    }

    // Sprite size
    if (lcdc[2] == 0) {
        spriteSize = 8;
    } else {
        spriteSize = 16;
    }

    // Tile data
    if (lcdc[4] == 0) {
        startAddress = 0x8800;
        isUnsignedByte = false;
    } else {
        startAddress = 0x8000;
    }

    // Calculate which tile to draw
    if (windowEnabled) {
        yPosition = mmu->readByte(0xFF44) - windowY;
    } else {
        yPosition = scrollY + mmu->readByte(0xFF44);
    }

    u16 tileRow = (yPosition / 8) * 32;
}

void Graphics::renderTiles() {
    std::vector<sf::Uint8> pixelBuffer(SCREEN_WIDTH * SCREEN_HEIGHT * 4, 255);
    for (int pixel = 0; pixel < SCREEN_WIDTH; pixel++) {
        u8 xPosition = pixel + scrollX;
        if (windowEnabled) {
            if (pixel >= windowX) {
                xPosition = pixel - windowX;
            }
        }

        // Get tile address
        u16 tileAddress = backgroundMemory + ((scanLineCounter / 8) * 32) + (xPosition / 8);
        u16 tileNumber = mmu->readByte(tileAddress);
        u16 tileLocation = startAddress;

        if (isUnsignedByte) {
            tileLocation += (tileNumber * 16);
        } else {
            tileLocation += ((tileNumber + 128) * 16);
        }

        // Note: each bit contains info for two adjacent pixels
        u8 line = (scanLineCounter % 8) * 2;
        u8 pixel1 = mmu->readByte(tileLocation + line);
        u8 pixel2 = mmu->readByte(tileLocation + line + 1);

        // Get pixel color
        int colorBit = (xPosition % 8) - 1;
        colorBit -= 7;
        colorBit *= -1;

        // Combine data to get color id for pixel
        int colorNum = (pixel2 & (1 << colorBit)) >> colorBit;
        colorNum <<= 1;
        colorNum |= (pixel1 & (1 << colorBit)) >> colorBit;

        switch(colorNum) {
            // White
            case 0:
                pixelBuffer[((line * SCREEN_WIDTH) + pixel) * 4] = 255;
                pixelBuffer[((line * SCREEN_WIDTH) + pixel) * 4 + 1] = 255;
                pixelBuffer[((line * SCREEN_WIDTH) + pixel) * 4 + 2] = 255;
                break;

            // Light grey
            case 1:
                pixelBuffer[((line * SCREEN_WIDTH) + pixel) * 4] = 0xCC;
                pixelBuffer[((line * SCREEN_WIDTH) + pixel) * 4 + 1] = 0xCC;
                pixelBuffer[((line * SCREEN_WIDTH) + pixel) * 4 + 2] = 0xCC;
                break;

            // Dark grey
            case 2:
                pixelBuffer[((line * SCREEN_WIDTH) + pixel) * 4] = 0x77;
                pixelBuffer[((line * SCREEN_WIDTH) + pixel) * 4 + 1] = 0x77;
                pixelBuffer[((line * SCREEN_WIDTH) + pixel) * 4 + 2] = 0x77;
                break;
            
            // Black
            case 3:
                pixelBuffer[((line * SCREEN_WIDTH) + pixel) * 4] = 0;
                pixelBuffer[((line * SCREEN_WIDTH) + pixel) * 4 + 1] = 0;
                pixelBuffer[((line * SCREEN_WIDTH) + pixel) * 4 + 2] = 0;
                break;
        }
    }

    // Update the texture with the entire pixelBuffer
    texture.update(pixelBuffer.data());
    // printf("Pixel buffer data: %d\n", pixelBuffer.data());
}

void Graphics::run() {
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        updateDisplay();
    }
}

void Graphics::renderSprites() {
    for (int sprite = 0; sprite < 40; sprite++) {
        u8 index = sprite * 4;
        u8 yPos = mmu->readByte(0xFE00 + index) - 16;
        u8 xPos = mmu->readByte(0xFE00 + index + 1) - 8;
        u8 tileLocation = mmu->readByte(0xFE00 + index + 2);
        u8 attributes = mmu->readByte(0xFE00 + index + 3);

        bool yFlip = (attributes & (1 << 6)) != 0;
        bool xFlip = (attributes & (1 << 5)) != 0;

        u8 scanLine = mmu->readByte(0xFF44);

        // Does sprite intercept with scanline?
        if ((scanLine >= yPos) && (scanLine < (yPos + spriteSize))) {
            int line = scanLine - yPos;

            // Read sprite backwards if yFlip is set
            if (yFlip) {
                line -= spriteSize;
                line *= -1;
            }

            line *= 2;
            u16 dataAddress = (0x8000 + (tileLocation * 16)) + line;
            u8 data1 = mmu->readByte(dataAddress);
            u8 data2 = mmu->readByte(dataAddress + 1);

            for (int tilePixel = 7; tilePixel >= 0; tilePixel--) {
                int colorBit = tilePixel;

                // Read sprite backwards if xFlip is set
                if (xFlip) {
                    colorBit -= 7;
                    colorBit *= -1;
                }

                int colorNum = (data2 & (1 << colorBit)) >> colorBit;
                colorNum <<= 1;
                colorNum |= (data1 & (1 << colorBit)) >> colorBit;

                // Get color from palette
                u8 color = getPixelColor(colorNum);
            }
        }
    }
}

sf::Uint8 Graphics::getPixelColor(u8 pixelValue) {
    switch (pixelValue) {
        case 0:
            return 00; // White
        case 1:
            return 01; // Light grey
        case 2:
            return 10; // Dark grey
        default:
            return 11;   //  Black
    }
}

std::vector<sf::Uint8> Graphics::updateScanline() {
    std::vector<sf::Uint8> scanline(SCREEN_WIDTH);

    // for (int pixel = 0; pixel < SCREEN_WIDTH; pixel++) {
    //     u16 offset = startAddress + pixel + (scanLineCounter - 1) * SCREEN_WIDTH;
    //     scanline[pixel] = mmu->readByte(offset);
    //     int color = getPixelColor(scanline[pixel]);
    //     scanline[pixel] = (color << 16) | (color << 8) | color | (255 << 24); 
    // }

    // return scanline;
}

void Graphics::updateArray(int cycles) {
    cycleCounter += cycles;
    printf("Cycle counter: %d\n", cycleCounter);

    // Reset cycle counter
    if (cycleCounter >= 456) {
        cycleCounter = cycleCounter % 456;
    }

    if (cycleCounter <= 456) {
        scanLineCounter++;

        // Set interrupt flag
        if (scanLineCounter == 144) {
            printf("Vblank\n");
            cpu->requestInterrupt(0x1);

        // Reset scanline counter
        } else if (scanLineCounter > 153) {
            scanLineCounter = 0;

        // Vdraw - update scanline array
        } else if (scanLineCounter < 144) {
            renderTiles();
        }
    }
    return;
}

void Graphics::updateDisplay() {
    window.clear();
    renderTiles();
    window.draw(sprite);
    window.display();
}

bool Graphics::isOpen() {
    return window.isOpen();
}

Graphics::~Graphics() {
}
