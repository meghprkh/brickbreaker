#include "background.h"

void Background::init()
{
    const int size = 7;
    for (int i = 0; i < 100; i++) {
        double x = ((i/size) % 2 == 0) ? 1+sin(M_PI/6) : 0;
        x += i%size*3;
        double y = i/size * cos(M_PI/6);
        hx[i] = Hexagon(glm::vec3(x-4, y-4, 0));
    }
}

void Background::draw(glm::mat4 VP) {
    for (int i = 0; i < 100; i++) hx[i].draw(VP);
}
