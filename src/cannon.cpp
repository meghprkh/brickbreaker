#include "cannon.h"
#include "main.h"

#define ROTATION_SPEED 2
#define TRANSLATION_SPEED 0.05

Cannon::Cannon(float rotation)
{
    this->rotation = rotation;
    this->y = 0;
    static const GLfloat vertex_buffer_data [] = {
         -1,  0.3, 0,
        0.2,  0.3, 0,
        0.2, -0.3, 0,
        0.2, -0.3, 0,
         -1, -0.3, 0,
         -1,  0.3, 0,

          -1,  0.125, 0,
        0.75,  0.125, 0,
        0.75, -0.125, 0,
        0.75, -0.125, 0,
          -1, -0.125, 0,
          -1,  0.125, 0,

        0.75,  0.125, 0,
        0.8,       0, 0,
        0.75, -0.125, 0,
    };

    static const GLfloat color_buffer_data [] = {
        COLOR_CANNON1.r / 255.0f, COLOR_CANNON1.g / 255.0f, COLOR_CANNON1.b / 255.0f,
        COLOR_CANNON1.r / 255.0f, COLOR_CANNON1.g / 255.0f, COLOR_CANNON1.b / 255.0f,
        COLOR_CANNON1.r / 255.0f, COLOR_CANNON1.g / 255.0f, COLOR_CANNON1.b / 255.0f,
        COLOR_CANNON1.r / 255.0f, COLOR_CANNON1.g / 255.0f, COLOR_CANNON1.b / 255.0f,
        COLOR_CANNON1.r / 255.0f, COLOR_CANNON1.g / 255.0f, COLOR_CANNON1.b / 255.0f,
        COLOR_CANNON1.r / 255.0f, COLOR_CANNON1.g / 255.0f, COLOR_CANNON1.b / 255.0f,

        COLOR_CANNON2.r / 255.0f, COLOR_CANNON2.g / 255.0f, COLOR_CANNON2.b / 255.0f,
        COLOR_CANNON2.r / 255.0f, COLOR_CANNON2.g / 255.0f, COLOR_CANNON2.b / 255.0f,
        COLOR_CANNON2.r / 255.0f, COLOR_CANNON2.g / 255.0f, COLOR_CANNON2.b / 255.0f,
        COLOR_CANNON2.r / 255.0f, COLOR_CANNON2.g / 255.0f, COLOR_CANNON2.b / 255.0f,
        COLOR_CANNON2.r / 255.0f, COLOR_CANNON2.g / 255.0f, COLOR_CANNON2.b / 255.0f,
        COLOR_CANNON2.r / 255.0f, COLOR_CANNON2.g / 255.0f, COLOR_CANNON2.b / 255.0f,

        COLOR_CANNON2.r / 255.0f, COLOR_CANNON2.g / 255.0f, COLOR_CANNON2.b / 255.0f,
        COLOR_CANNON2.r / 255.0f, COLOR_CANNON2.g / 255.0f, COLOR_CANNON2.b / 255.0f,
        COLOR_CANNON2.r / 255.0f, COLOR_CANNON2.g / 255.0f, COLOR_CANNON2.b / 255.0f,
    };

    this->object = create3DObject(GL_TRIANGLES, 15, vertex_buffer_data, color_buffer_data, GL_FILL);
}


void Cannon::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (glm::vec3(-4, this->y, 0));        // glTranslatef
    glm::mat4 rotate = glm::rotate((float) (this->rotation*M_PI/180.0f), glm::vec3(0,0,1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Cannon::rotate(direction_t dir) {
    if (dir == DIR_UP && this->rotation < 90) this->rotation += ROTATION_SPEED;
    else if (dir == DIR_DOWN && this->rotation > -90) this->rotation -= ROTATION_SPEED;
}

void Cannon::move(direction_t dir) {
    if (dir == DIR_UP && this->y < 4) this->y += TRANSLATION_SPEED;
    else if (dir == DIR_DOWN && this->y > -4) this->y -= TRANSLATION_SPEED;
}
