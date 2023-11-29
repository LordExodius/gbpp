#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>
#include "MMU.h"
#include "CPU.h"

class Graphics {
    public:
        /**
         * @brief Construct a new Graphics object
         * 
         * @param mmu 
         */
        Graphics(MMU* mmu); 

        /**
         * @brief Destroy the Graphics object
         */
        ~Graphics();

        /**
         * @brief Runs the graphics of the emulator and calls function to update the display
         */
        void run();
        /**
         * @brief Updates the display of the emulator
         */
        void updateDisplay();

        /**
         * @brief A function to check if the window is open
         * 
         * @return true if the window is open
         * @return false if the window is not open
         */
        bool isOpen();

        /**
         * @brief Determines the graphics should do and calls the function to update the scanline array
         * 
         * @param cycles The number of cycles that passed since the last update
         * @return std::vector<sf::Uint8> An array of pixels representing the scanline
         */
        std::vector<sf::Uint8> updateArray(int cycles);

    private:
        int spriteSize; ///< The size of the sprites, either 8x8 or 8x16
        int cycleCounter; ///< The number of cycles that have passed since the last update
        int scanLineCounter; ///< The current scanline
        u8 scrollX; ///< The x position of the scroll
        u8 scrollY; ///< The y position of the scroll
        u8 windowX; ///< The x position of the window
        u8 windowY; ///< The y position of the window
        bool windowEnabled; ///< Whether the window is enabled
        bool isUnsignedByte; ///< Whether the tile data is signed or unsigned
        u16 startAddress; ///< The start address of the tile data
        u16 backgroundMemory; ///< The start address of the background memory
        const int SCREEN_WIDTH = 160; ///< The width of the screen
        const int SCREEN_HEIGHT = 144; ///< The height of the screen
        sf::RenderWindow window; ///< The window of the emulator
        sf::Texture texture; ///< The texture of the emulator
        sf::Sprite sprite; ///< The sprite of the emulator
        // CPU* cpu; ///< A pointer to the CPU object
        MMU* mmu; ///< A pointer to the MMU object

        /**
         * @brief Get the Pixel color
         * 
         * @param pixelValue An u8 value representing the pixel
         * @return sf::Uint8 The pixel color (white, light grey, dark grey, black)
         */
        sf::Uint8 getPixelColor(u8 pixelValue);

        /**
         * @brief A function to update the scanline array
         * 
         * @return std::vector<sf::Uint8> The updated scanline array
         */
        std::vector<sf::Uint8> updateScanline();

        /**
         * @brief Set the Initial Display based on the settings specified in the LCD Control Register
         */
        void setInitialDisplay();

        /**
         * @brief Calculating the tile of the background graphics
         */
        void renderTiles();

        /**
         * @brief Calculates the position, display, and color of the sprite graphics
         */
        void renderSprites();
};

#endif
