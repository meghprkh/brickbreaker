#include "main.h"

#ifndef MIRROR_H
#define MIRROR_H


class Mirror
{
public:
    Mirror() {};
    Mirror(float x, float y, float rotation);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO* object;
};

#endif // MIRROR_H
