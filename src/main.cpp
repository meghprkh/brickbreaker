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
#include "digit.h"
#include "score.h"

#define NUM_MIRRORS 4
#define MAX_BRICKS 100

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow* window;

/**************************
 * Customizable functions *
 **************************/

Mirror mirrors[NUM_MIRRORS];
Brick bricks[MAX_BRICKS];
bool bricks_present[MAX_BRICKS];
Basket red_basket, green_basket;
Cannon cannon;
extern bool cannon_keyboard_input;
Laser lasers[MAX_BRICKS];
bool lasers_present[MAX_BRICKS];
int laser_cooldown = 0;
Score score;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

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
  for (int i = 0; i < NUM_MIRRORS; i++) mirrors[i].draw(VP);

  for (int i = 0; i < MAX_BRICKS; i++)
      if (lasers_present[i])
          lasers[i].draw(VP);
  for (int i = 0; i < MAX_BRICKS; i++)
      if (bricks_present[i])
          bricks[i].draw(VP);

  red_basket.draw(VP);
  green_basket.draw(VP);
  cannon.draw(VP);
  score.draw(VP);
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
    } else {
        if (left) { screen_center_x -= 0.05; reset_screen(); }
        if (right) { screen_center_x += 0.05; reset_screen(); }
        if (glfwGetKey(window, GLFW_KEY_UP)) { screen_center_y += 0.05; reset_screen(); }
        if (glfwGetKey(window, GLFW_KEY_DOWN)) { screen_center_y -= 0.05; reset_screen(); }
    }

    if (cannon_keyboard_input) {
        if (glfwGetKey(window, GLFW_KEY_A)) cannon.rotate(DIR_UP);
        if (glfwGetKey(window, GLFW_KEY_D)) cannon.rotate(DIR_DOWN);
    } else {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        if (0 < xpos && xpos < width && 0 < ypos && ypos < height) {
            double xdiff = xpos*8/width;
            double ydiff = (height-ypos)*8/height - 4 - cannon.y;
            int rot = atan(ydiff/xdiff)*180/M_PI;
            cannon.rotation = rot;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_S)) cannon.move(DIR_UP);
    if (glfwGetKey(window, GLFW_KEY_F)) cannon.move(DIR_DOWN);

    if (glfwGetKey(window, GLFW_KEY_SPACE)) shoot_laser(-4, cannon.y, cannon.rotation);
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
                score.add();
            } else if (red_collision || green_collision) {
                if (bricks[i].color == BRICK_BLACK) quit(window);
                score.subtract();
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

    mirrors[0] = Mirror(0, 3, 60);
    mirrors[1] = Mirror(0, -3, -60);
    mirrors[2] = Mirror(2, 1, 30);
    mirrors[3] = Mirror(2, -1, -30);

    for (int i = 0; i < MAX_BRICKS; i++) {
        bricks_present[i] = false;
        lasers_present[i] = false;
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
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0 , 0.0f); // R, G, B, A
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

    window = initGLFW(width, height);

	initGL (window, width, height);

    Timer t60(1.0/60), brickTimer(2);

    score.update(0);

    audio_init();

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        audio_play();
        // Process timers

        if (t60.processTick()) {
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);

            if (laser_cooldown > 0) {
                laser_cooldown--;
                if (laser_cooldown == 55)
                    for (int i = 0; i < MAX_BRICKS; i++)
                        lasers_present[i] = false;
            }
        }

        if (brickTimer.processTick()) {
            for (int i = 0; i < MAX_BRICKS; i++) {
                if (!bricks_present[i]) {
                    bricks[i] = Brick(static_cast<brick_color_t>(rand() % 3));
                    bricks[i].set_position(((double) rand()/RAND_MAX - 0.5)*7+0.5, 4.5);
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

void shoot_laser(double x, double y, double rotation, int from_mirror)
{
    if (laser_cooldown > 0) return;
    int laseri = 0;
    for (; laseri < MAX_BRICKS; laseri++)
        if (!lasers_present[laseri]) break;
    lasers[laseri] = Laser(x, y, rotation);
    lasers_present[laseri] = true;
    double minx = 10;
    int mini = -1;
    bool ismirror = false;
    for (int i = 0; i < MAX_BRICKS; i++) {
        if (bricks_present[i] && lasers[laseri].collides(bricks[i].bounding_box())) {
            double dist = abs(x - bricks[i].position.x);
            if (minx > dist) {
                minx = dist;
                mini = i;
            }
        }
    }
    for (int i = 0; i < NUM_MIRRORS; i++) {
        if (from_mirror == i) continue;
        if (90 + mirrors[i].rotation == rotation) continue;
        float m1 = tan((90 + mirrors[i].rotation) * M_PI / 180);
        float m2 = tan(rotation * M_PI / 180);
        float c1 = mirrors[i].position.y - mirrors[i].position.x * m1;
        float c2 = y - x * m2;
        float xint = (c2 - c1) / (m1 - m2);
        float yint = m1 * xint + c1;
        float ldist = sqrt((xint-mirrors[i].position.x)*(xint-mirrors[i].position.x) + (yint - mirrors[i].position.y)*(yint - mirrors[i].position.y));
        if (ldist <= 0.6) {
            double dist = abs(xint - x);
            if (minx > dist) {
                minx = dist;
                mini = i;
                ismirror = true;
            }
        }
    }
    if (ismirror) {
        int i = mini;
        float m1 = tan((90 + mirrors[i].rotation) * M_PI / 180);
        float m2 = tan(rotation * M_PI / 180);
        float c1 = mirrors[i].position.y - mirrors[i].position.x * m1;
        float c2 = y - x * m2;
        float xint = (c2 - c1) / (m1 - m2);
        float yint = m1 * xint + c1;
        float roti = 2*(90 + mirrors[i].rotation) - rotation;
        lasers[laseri].createObject(xint, yint);
        shoot_laser(xint, yint, roti, i);
    } else if (mini == -1) {
        laser_cooldown = 60;
        lasers[laseri].createObject();
//        score -= 1;
    } else {
        laser_cooldown = 60;
        if (bricks[mini].color == BRICK_BLACK) score.add();
        else score.subtract();
        lasers[laseri].createObject(bricks[mini].bounding_box());
        bricks[mini] = Brick();
        bricks_present[mini] = false;
    }
}

void reset_screen()
{
    float top = (screen_center_y+4) / screen_zoom;
    float bottom = (screen_center_y-4) / screen_zoom;
    float left = (screen_center_x-4) / screen_zoom;
    float right = (screen_center_x+4) / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
