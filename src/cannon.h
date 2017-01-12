#ifndef CANNON_H
#define CANNON_H

#include "main.h"

class Cannon
{
public:
    Cannon() {}
    Cannon(float rotation);
    float rotation;
    void draw(glm::mat4 VP);
    void rotate(direction_t dir);
    void move(direction_t dir);
    double y;
private:
    VAO* object;
};

#endif // CANNON_H
