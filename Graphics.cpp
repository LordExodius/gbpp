#include "Graphics.h"
#include <iostream>
#include <bitset>

Graphics::Graphics(MMU* mmu, CPU* cpu, sf::RenderWindow* window) {
    this->cpu = cpu;
    this->mmu = mmu;
    this->window = window; 
    this->window->setFramerateLimit(60); 
    // texture.create(SCREEN_WIDTH, SCREEN_HEIGHT);
    setInitialDisplay();
}

void Graphics::setInitialDisplay() {
    // Read LCDC register for initial display configuration
    u8 u8lcdc = mmu->readByte(0xFF40);
    std::bitset<8> lcdc(u8lcdc);
    // printf("LCDC: %d\n", lcdc);

    // Determine visual area and window
    scrollY = mmu->readByte(0xFF42);
    scrollX = mmu->readByte(0xFF43);
    windowY = mmu->readByte(0xFF4A);
    windowX = mmu->readByte(0xFF4B) - 7;

    // printf("Window X: %d\n", windowX);
    // printf("Window Y: %d\n", windowY);
    // printf("Scroll X: %d\n", scrollX);
    // printf("Scroll Y: %d\n", scrollY);

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

        if (isUnsignedByte) {
            startAddress += (pixel * 16);
        } else {
            startAddress += ((pixel + 128) * 16);
        }

        // Note: each bit contains info for two adjacent pixels
        u8 line = (scanLineCounter % 8) * 2;
        u8 pixel1 = mmu->readByte(startAddress + line);
        u8 pixel2 = mmu->readByte(startAddress + line + 1);

        // Print the line
        // printf("Line: %d\n", line);
        // printf("Scanline Counter: %d\n", scanLineCounter);

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
    sf::IntRect updateRect(0, scanLineCounter, SCREEN_WIDTH, 1);
    texture.update(pixelBuffer.data(), SCREEN_WIDTH, 1, 0, scanLineCounter);
    // printf("Scanline counter: %d\n", scanLineCounter);

    // printf("Pixel buffer data: ");
    // for (int i = 0; i < 10; ++i) {
    //     printf("%u ", static_cast<unsigned int>(pixelBuffer[i]));
    // }
    // printf("\n");
}

void Graphics::updateArray(int cycles) {
    cycleCounter += cycles;
    // printf("Cycles param: %d\n", cycles);
    // printf("Cycle counter: %d\n", cycleCounter);

    // Reset cycle counter
    if (cycleCounter >= 456) {
        cycleCounter = cycleCounter % 456;
    }

    if (cycleCounter <= 456) {
        scanLineCounter++;

        // Set interrupt flag
        if (scanLineCounter == 144) {
            // printf("Vblank\n");
            cpu->requestInterrupt(0x1);

        // Reset scanline counter
        } else if (scanLineCounter > 153) {
            scanLineCounter = 0;
            window->clear();
        // Vdraw - update scanline array
        } else if (scanLineCounter < 144) {
            renderTiles();
            // updateDisplay();
        }
    }
    return;
}

void Graphics::updateDisplay() {
    // renderTiles();
    window->clear();
    window->draw(sprite);
    window->display();
}

bool Graphics::isOpen() {
    return window->isOpen();
}

Graphics::~Graphics() {
}
