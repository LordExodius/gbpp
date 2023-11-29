#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "global.h"
#include "MMU.h"
#include <bitset>
#include <unordered_map>
#include <iostream>

class Input {
private:
    MMU& mmu; ///< Reference to the MMU object
    std::bitset<4> buttonKeys; ///< Bitset representing the button keys
    std::bitset<4> directionKeys; ///< Bitset representing the direction keys
    std::unordered_map<char, int> buttonKeyMappings; ///< Maps keyboard keys to button keys
    std::unordered_map<char, int> directionKeyMappings; ///< Maps keyboard keys to direction keys

    /**
     * @brief Updates the joypad register with the current state of the button and direction keys
     */
    void updateJoypadRegister();

public:
    /**
     * @brief Construct a new Input object
     * 
     * @param mmu 
     */
    Input(MMU& mmu);

    /**
     * @brief Destroy the Input object
     */
    ~Input();
    void pressKey(char key); ///< Presses a key on the keyboard
    void releaseKey(char key); ///< Releases a key on the keyboard

    /**
     * @brief Get the Button Keys State object
     * 
     * @return An u8 value which represents the state of the button keys
     */
    u8 getButtonKeysState();

    /**
     * @brief Get the Direction Keys State object
     * 
     * @return An u8 value which represents the state of the direction keys
     */
    u8 getDirectionKeysState();
};
#endif