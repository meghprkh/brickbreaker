#include "laser.h"
#include "main.h"

Laser::Laser(double x, double y, double rotation)
{
    this->x = x;
    this->y = y;
    this->rotation = rotation;

}

bool Laser::collides(bounding_box_t bbox) {
    double angle = atan((bbox.y - y) / (bbox.x - x)) * 180 / M_PI ;
    return abs(angle - rotation) < 5;
}

void Laser::createObject(bounding_box_t bbox) {
    const GLfloat vertex_buffer_data [] = {
        x, y, 0, bbox.x, bbox.y, 0
    };
    this->object = create3DObject(GL_LINES, 2, vertex_buffer_data, 0.7, 0.25, 0.25, GL_LINE);
}

void Laser::createObject() {
    const GLfloat vertex_buffer_data [] = {
        x, y, 0, 15*cos(M_PI*rotation/180), 15*sin(M_PI*rotation/180), 0
    };
    this->object = create3DObject(GL_LINES, 2, vertex_buffer_data, 0.7, 0.25, 0.25, GL_LINE);
}

void Laser::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
