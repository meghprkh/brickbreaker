#ifndef LASER_H
#define LASER_H

#include "main.h"

class Laser
{
public:
    Laser() {}
    Laser(double x, double y, double rotation);
    bool collides(bounding_box_t bbox);
    void createObject(float dist = 15);
    void createObject(bounding_box_t bbox);
    void draw(glm::mat4 VP);
    double x, y, rotation;
private:
    VAO* object;
    double pointSign(double x, double y);
};

#endif // LASER_H
