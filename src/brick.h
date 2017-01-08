#include "main.h"
#ifndef BRICK_H
#define BRICK_H


class Brick
{
public:
    Brick() {};
    Brick(brick_color_t color);
    brick_color_t color;
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO* object;
};

#endif // BRICK_H
