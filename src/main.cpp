#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "main.h"
#include "brick.h"
#include "mirror.h"
#include "basket.h"
#include "timer.h"
#include "cannon.h"
#include "laser.h"

#define MAX_BRICKS 100

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow* window;

/**************************
 * Customizable functions *
 **************************/

Mirror m1, m2;
Brick bricks[MAX_BRICKS];
bool bricks_present[MAX_BRICKS];
Basket red_basket, green_basket;
Cannon cannon;
Laser laser;
bool laser_present = false;
int laser_cooldown = 0;

int score;

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw ()
{
  // clear the color and depth in the frame buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // use the loaded shader program
  // Don't change unless you know what you are doing
  glUseProgram (programID);

  // Eye - Location of camera. Don't change unless you are sure!!
  // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
  // Target - Where is the camera looking at.  Don't change unless you are sure!!
  // glm::vec3 target (0, 0, 0);
  // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
  // glm::vec3 up (0, 1, 0);

  // Compute Camera matrix (view)
  // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
  //  Don't change unless you are sure!!
  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane

  // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
  //  Don't change unless you are sure!!
  glm::mat4 VP = Matrices.projection * Matrices.view;

  // Send our transformation to the currently bound shader, in the "MVP" uniform
  // For each model you render, since the MVP will be different (at least the M part)
  //  Don't change unless you are sure!!
  glm::mat4 MVP;	// MVP = Projection * View * Model

  // Scene render
  m1.draw(VP);
  m2.draw(VP);

  if (laser_present) laser.draw(VP);
  for (int i = 0; i < MAX_BRICKS; i++)
      if (bricks_present[i])
          bricks[i].draw(VP);

  red_basket.draw(VP);
  green_basket.draw(VP);
  cannon.draw(VP);
}

void tick_input(GLFWwindow* window) {
    int left = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT) || glfwGetKey(window, GLFW_KEY_RIGHT_ALT);
    int ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
    if (ctrl) {
        if (left) red_basket.move(DIR_LEFT);
        else if (right) red_basket.move(DIR_RIGHT);
    } else if (alt) {
        if (left) green_basket.move(DIR_LEFT);
        else if (right) green_basket.move(DIR_RIGHT);
    }

    if (glfwGetKey(window, GLFW_KEY_A)) cannon.rotate(DIR_UP);
    if (glfwGetKey(window, GLFW_KEY_D)) cannon.rotate(DIR_DOWN);

    if (glfwGetKey(window, GLFW_KEY_S)) cannon.move(DIR_UP);
    if (glfwGetKey(window, GLFW_KEY_F)) cannon.move(DIR_DOWN);

    if (glfwGetKey(window, GLFW_KEY_SPACE)) shoot_laser();
}

void tick_elements() {
    for (int i = 0; i < MAX_BRICKS; i++)
        if (bricks_present[i]) {
            bricks[i].tick();

            bounding_box_t bbox = bricks[i].bounding_box();
            bool red_collision = detect_collision(bbox, red_basket.bounding_box());
            bool green_collision = detect_collision(bbox, green_basket.bounding_box());
            if ((red_collision && bricks[i].color == BRICK_RED) ||
                (green_collision && bricks[i].color == BRICK_GREEN)) {
                score += 2;
            } else if (red_collision || green_collision) {
                if (bricks[i].color == BRICK_BLACK) quit(window);
                score -= 1;
            }

            if (red_collision || green_collision || bricks[i].position.y < -4.5) {
                bricks[i] = Brick();
                bricks_present[i] = false;
            }
        }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
	// Create the models

    m1 = Mirror(0, 0, 0);
    m2 = Mirror(0, 0, -45);

    for (int i = 0; i < MAX_BRICKS; i++) {
        bricks_present[i] = false;
    }

    red_basket = Basket(BRICK_RED);
    red_basket.set_position(3, -3.5);
    green_basket = Basket(BRICK_GREEN);
    green_basket.set_position(-3, -3.5);

    cannon = Cannon(0);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


	reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (1, 1, 1, 0.0f); // R, G, B, A
	glClearDepth (1.0f);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main (int argc, char** argv)
{
    srand(time(0));
	int width = 600;
	int height = 600;
    score = 0;

    window = initGLFW(width, height);

	initGL (window, width, height);

    Timer tquarter(0.25), t1s(1), t60(1.0/60), brickTimer(2);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        if (tquarter.processTick()) {
            laser_present = false;
        }

        if (t1s.processTick()) {
            // happens every 1 second
            if (laser_cooldown > 0) laser_cooldown--;
        }

        if (brickTimer.processTick()) {
            for (int i = 0; i < MAX_BRICKS; i++) {
                if (!bricks_present[i]) {
                    bricks[i] = Brick(static_cast<brick_color_t>(rand() % 3));
                    bricks[i].set_position(((double) rand()/RAND_MAX - 0.5)*8, 4.5);
                    bricks_present[i] = true;
                    break;
                }
            }
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void shoot_laser()
{
    if (laser_present || laser_cooldown > 0) return;
    laser_present = true;
    laser_cooldown = 1;
    laser = Laser(-4+0.8*cos(M_PI*cannon.rotation/180), cannon.y + 0.8*sin(M_PI*cannon.rotation/180), cannon.rotation);
    double minx = 5;
    int mini = -1;
    for (int i = 0; i < MAX_BRICKS; i++) {
        if (bricks_present[i] && laser.collides(bricks[i].bounding_box())) {
            if (minx > bricks[i].position.x) {
                minx = bricks[i].position.x;
                mini = i;
            }
        }
    }
    if (mini == -1) {
        laser.createObject();
//        score -= 1;
    } else {
        if (bricks[mini].color == BRICK_BLACK) score += 2;
        else score -= 1;
        laser.createObject(bricks[mini].bounding_box());
        bricks[mini] = Brick();
        bricks_present[mini] = false;
    }
}
