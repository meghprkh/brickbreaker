#include "brick.h"
#include "main.h"

Brick::Brick(brick_color_t color)
{
    this->color = color;
    // GL3 accepts only Triangles. Quads are not supported
    static const GLfloat vertex_buffer_data [] = {
      -0.2,-0.2,0, // vertex 1
      0.2,-0.2,0, // vertex 2
      0.2, 0.2,0, // vertex 3

      0.2, 0.2,0, // vertex 3
      -0.2, 0.2,0, // vertex 4
      -0.2,-0.2,0  // vertex 1
    };

    static GLfloat color_data_red [6*3] = {
        1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0
    };
    static GLfloat color_data_green [6*3] = {
        0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0
    };
    static GLfloat color_data_black [6*3] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    static GLfloat *color_buffer_data;
    if (color == BRICK_RED) color_buffer_data = color_data_red;
    else if (color == BRICK_GREEN) color_buffer_data = color_data_green;
    else color_buffer_data = color_data_black;

    // create3DObject creates and returns a handle to a VAO that can be used later
    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}

void Brick::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translateRectangle = glm::translate (this->position);        // glTranslatef
    Matrices.model *= translateRectangle;
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Brick::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
