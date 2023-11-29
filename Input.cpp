#include "Input.h"

Input::Input(MMU& mmu) : mmu(mmu){
    //  Initialize to unpressed state - note: for GameBoys, 0 = pressed, 1 = unpressed
    buttonKeys.set();
    directionKeys.set();

    // Buttonn key mappings
    buttonKeyMappings['1'] = 0; // A
    buttonKeyMappings['2'] = 1; // B

    // Find alternate keys? Doesn't really work too well on the keyboard
    buttonKeyMappings['\n'] = 2; // Map Start to Enter
    buttonKeyMappings[' '] = 3; // Map Select to Spacebar

    // Direction key mappings
    directionKeyMappings['a'] = 1; // Left
    directionKeyMappings['w'] = 2; // Up
    directionKeyMappings['s'] = 3; // Down
    directionKeyMappings['d'] = 0; // Right
}

void Input::pressKey(char key) {
    if (buttonKeyMappings.find(key) != buttonKeyMappings.end()) {
        buttonKeys.reset(buttonKeyMappings[key]);
    } 
    
    // "Select" button must be pressed while reading keys from the lower nibble
    else if (directionKeyMappings.find(key) != directionKeyMappings.end()) {
        buttonKeys.reset(buttonKeyMappings[' ']);
        directionKeys.reset(directionKeyMappings[key]);
    }

    updateJoypadRegister();
}
void Input::releaseKey(char key) {
    if (buttonKeyMappings.find(key) != buttonKeyMappings.end()) {
        buttonKeys.set(buttonKeyMappings[key]);
    } 
    
    else if (directionKeyMappings.find(key) != directionKeyMappings.end()) {
        buttonKeys.set(buttonKeyMappings[' ']);
        directionKeys.set(directionKeyMappings[key]);
    }

    updateJoypadRegister();
}

u8 Input::getButtonKeysState() {
    return static_cast<u8>(buttonKeys.to_ulong());
}

u8 Input::getDirectionKeysState() {
    return static_cast<u8>(directionKeys.to_ulong());
}

void Input::updateJoypadRegister() {
    mmu.writeByte(0xFF00, (getDirectionKeysState() << 4) | getButtonKeysState());
}