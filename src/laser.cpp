#include "laser.h"
#include "main.h"

Laser::Laser(double x, double y, double rotation)
{
    this->x = x;
    this->y = y;
    this->rotation = rotation;
}


double Laser::pointSign(double x, double y) {
    return (y - this->y) - tan(M_PI * this->rotation / 180)*(x - this->x);
}


bool Laser::collides(bounding_box_t bbox) {
    return (
        (pointSign(bbox.x - bbox.width/2, bbox.y - bbox.height/2)*pointSign(bbox.x + bbox.width/2, bbox.y + bbox.height/2) <= 0) ||
        (pointSign(bbox.x - bbox.width/2, bbox.y + bbox.height/2)*pointSign(bbox.x + bbox.width/2, bbox.y - bbox.height/2) <= 0)
    );
}

void Laser::createObject(bounding_box_t bbox) {
    float dist = sqrt((y-bbox.y)*(y-bbox.y) + (x-bbox.x)*(x-bbox.x));
    createObject(dist);
}

void Laser::createObject(float dist) {
    const GLfloat vertex_buffer_data [] = {
        (float) x, (float) y, 0,
        (float) (x + dist*cos(M_PI*rotation/180)), (float) (y + dist*sin(M_PI*rotation/180)), 0
    };
    this->object = create3DObject(GL_LINES, 2, vertex_buffer_data, 0.7, 0.25, 0.25, GL_LINE);
}

void Laser::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
