#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

class Graphics
{
private:
    int displayWidth;
    int displayHeight;
    int **frameBuffer;
    int *colorPalette;

public:
    Graphics(int width, int height);
    ~Graphics();
    void initDisplay();
    void updateDisplay();
};

#endif