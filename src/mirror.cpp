#include "mirror.h"
#include "main.h"

Mirror::Mirror(float x, float y, float rotation)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = rotation;
    static const GLfloat vertex_buffer_data [] = {
        0.0,    0, 0,    0,  1.2, 0,
        0.05, 0.15, 0, 0.2,  0.3, 0,
        0.05,  0.3, 0, 0.2, 0.45, 0,
        0.05, 0.45, 0, 0.2,  0.6, 0,
        0.05,  0.6, 0, 0.2, 0.75, 0,
        0.05, 0.75, 0, 0.2,  0.9, 0,
        0.05,  0.9, 0, 0.2, 1.05, 0,
        0.05, 1.05, 0, 0.2,  1.2, 0,
        0.05,  1.2, 0, 0.2, 1.35, 0,
    };

    static const GLfloat color_buffer_data [] = {
        0.1, 0.2, 0.7, 0.1, 0.2, 0.7,
        0.1, 0.2, 0.7, 0.1, 0.2, 0.7,
        0.1, 0.2, 0.7, 0.1, 0.2, 0.7,
        0.1, 0.2, 0.7, 0.1, 0.2, 0.7,
        0.1, 0.2, 0.7, 0.1, 0.2, 0.7,
        0.1, 0.2, 0.7, 0.1, 0.2, 0.7,
        0.1, 0.2, 0.7, 0.1, 0.2, 0.7,
        0.1, 0.2, 0.7, 0.1, 0.2, 0.7,
        0.1, 0.2, 0.7, 0.1, 0.2, 0.7
    };
    this->object = create3DObject(GL_LINES, 18, vertex_buffer_data, color_buffer_data, GL_LINE);
}

void Mirror::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);        // glTranslatef
    glm::mat4 rotate = glm::rotate((float) (this->rotation*M_PI/180.0f), glm::vec3(0,0,1));
    rotate = glm::translate(glm::vec3(0, 0.6, 0)) * rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Mirror::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
