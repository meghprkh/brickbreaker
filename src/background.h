#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "main.h"
#include "hexagon.h"

class Background
{
public:
    Background() {}
    void init();
    void draw(glm::mat4 VP);
private:
    Hexagon hx[100];
};

#endif // BACKGROUND_H
