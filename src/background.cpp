#include "background.h"
#include "main.h"

Background::Background(double rotation)
{
    this->rotation = rotation;
    scale = 1;
    static const GLfloat vertex_buffer_data [] = {
        1, 0, 0,
        cos(M_PI/3), sin(M_PI/3), 0,
        cos(2*M_PI/3), sin(2*M_PI/3), 0,
        cos(3*M_PI/3), sin(3*M_PI/3), 0,
        cos(4*M_PI/3), sin(4*M_PI/3), 0,
        cos(5*M_PI/3), sin(5*M_PI/3), 0,
    };

    static const GLfloat colour_buffer_data [] = {
        1, 0, 0,
        .5, .5, 0,
        0, 1, 0,
        0, .5, .5,
        0, 0, 1,
        .5, 0, .5
    };

    this->object = create3DObject(GL_LINE_LOOP, 6, vertex_buffer_data, colour_buffer_data, GL_LINE);
}

void Background::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 rotate = glm::rotate((float) (this->rotation*M_PI/180.0f), glm::vec3(0,0,1));
    int sf = (int) scale % 5 + 1;
    Matrices.model *=  rotate * glm::scale(glm::vec3(sf, sf, 1));
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Background::tick() {
    this->rotation -= 5;
    this->scale = (this->scale + 1) % 5;
}
