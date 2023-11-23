#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <bitset>
#include <unordered_map>

class Input
{
private:
    std::bitset<8> keys;
    std::unordered_map<char, int> keyMappings;

public:
    Input();
    void pressKey(char key);
    void releaseKey(char key);
    uint8_t getInputState();
};
#endif