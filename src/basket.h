#include "main.h"

#ifndef BASKET_H
#define BASKET_H

#define BASKET_SPEED 0.05

class Basket
{
public:
    Basket() {};
    Basket(brick_color_t color);
    brick_color_t color;
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void move(direction_t dir);
    bounding_box_t bounding_box();
private:
    VAO* object;
};

#endif // BASKET_H
