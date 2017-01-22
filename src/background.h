#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "main.h"

class Background
{
public:
    Background() {}
    Background(double rotation);
    void draw(glm::mat4 VP);
    void tick();
private:
    VAO* object;
    double rotation;
    int scale;
};

#endif // BACKGROUND_H
