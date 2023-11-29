#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "global.h"
#include "MMU.h"
#include <bitset>
#include <unordered_map>
#include <iostream>

class Input {
private:
    MMU& mmu;
    std::bitset<4> buttonKeys;
    std::bitset<4> directionKeys;
    std::unordered_map<char, int> buttonKeyMappings;
    std::unordered_map<char, int> directionKeyMappings;
    void updateJoypadRegister();

public:
    Input(MMU& mmu);
    void pressKey(char key);
    void releaseKey(char key);
    u8 getButtonKeysState();
    u8 getDirectionKeysState();
};
#endif