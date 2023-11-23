#include "input.h"

Input::Input()
{
    keyMappings['w'] = 0;
    keyMappings['s'] = 1;
    keyMappings['a'] = 2;
    keyMappings['d'] = 3;
    keyMappings['k'] = 4;
    keyMappings['l'] = 5;
    keyMappings['j'] = 6;
    keyMappings['Enter'] = 7;
}
void Input::pressKey(char key)
{
    if (keyMappings.find(key) != keyMappings.end())
    {
        keys.set(keyMappings[key]);
    }
}
void Input::releaseKey(char key)
{
    if (keyMappings.find(key) != keyMappings.end())
    {
        keys.reset(keyMappings[key]);
    }
}
uint8_t Input::getInputState()
{
    return static_cast<uint8_t>(keys.to_ulong());
}