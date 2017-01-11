#include "main.h"
#ifndef BRICK_H
#define BRICK_H


class Brick
{
public:
    Brick() {};
    Brick(brick_color_t color, float speed = 0.025);
    brick_color_t color;
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float speed;
    bounding_box_t bounding_box();
private:
    VAO* object;
};

#endif // BRICK_H
