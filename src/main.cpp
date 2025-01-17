/*

Author: Meghan Grayson
Last Modified Date: December 1, 2024

Compile with: make
Run with: ./WindowedBouncyBall
          ./WindowedBouncyBall <radius>
          ./WindowedBouncyBall <radius> <width> <height>
          ./WindowedBouncyBall <radius> <width> <height> <gravity (0 or 1)>

Source for some of the shader logic: LearnOpenGL.com

*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Circle.hpp"
#include "Shader.h"

/* Some stuff we need for generating the circle vertices */
const double PI = 3.1415926;
const unsigned int segments = 200;
float vertices[3*segments+1]; // "plus 1" because we have to include the center vertex

/* Defaults if no args provided */
int win_width = 300;
int win_height = 300;
int radius = 40;
bool gravity = true;

/* Generate the vertices to be stored in the buffer */
/* A filled circle is generated with GL_TRIANGLE_FAN, so we need to add the center point first */
void draw_circle(float cx, float cy, float r) {
    vertices[0] = cx; // Add the center of the circle
    vertices[1] = cy;
    vertices[2] = 0.0f;

    float theta, x, y;
    for(int i = 1; i <= segments+1; i++) { // Add the rest
        theta = 2.0f * PI * float(i-1)/float(segments); // current angle
        x = r * cosf(theta) + cx;
        y = r * sinf(theta) + cy;
        vertices[3*i] = x;
        vertices[3*i + 1] = y;
        vertices[3*i + 2] = 0.0f; // Z component is always 0
    }
}

int main(int argc, char** argv) {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Do not allow resizing

    // Check for args
    if(argc >=2) {
        // Args are: radius, width, height
        // Only change the defaults if values make sense
        int temp_rad = atoi(argv[1]);
        if(temp_rad >= 10 && temp_rad <= 100) radius = temp_rad;
        else std::cout<< "Please choose a radius between 10 and 150" << std::endl;

        if(argc >=4) {
            int temp_w = atoi(argv[2]);
            int temp_h = atoi(argv[3]);

            // make sure the values are even too
            if((temp_w >= 200 && temp_w <= 1200) && temp_w % 2 == 0) win_width = temp_w;
            else std::cout<< "Please choose an even-valued width between 200 and 1200" << std::endl;
            if((temp_h >= 200 && temp_h <= 1200) && temp_h % 2 == 0) win_height = temp_h;
            else std::cout<< "Please choose an even-valued height between 200 and 1200" << std::endl;

            if(argc >=5) {
                int temp_g = atoi(argv[4]);

                if(temp_g == 0 || temp_g == 1) gravity = temp_g;
                else std::cout<< "Please use 0 or 1 for gravity on or off" << std::endl;
            }
        }
    }

    // Print the radius and window dimensions
    std::cout<< "Radius: " << radius << std::endl;
    std::cout<< "Window dimensions: " << win_width << " x " << win_height << std::endl;
    std::cout<< "Gravity: " << gravity << std::endl;

    // Create the window
    GLFWwindow* window = glfwCreateWindow(win_width, win_height, "Windowed Bouncy Ball", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit(); // Have to init glew to use shaders

    // Use shader programs
    Shader ourShader("./shader/basic.vert", "./shader/basic.frag");

    // First get the circle vertices
    int array_size = 3*segments+1;
    float centerx = 0.0f;
    float centery = 0.0f;
    draw_circle(centerx, centery, radius); // Fill vertices array

    /* Initialize the ball */
    Circle *ball = new Circle(radius, glm::vec2(centerx, centery), win_width, win_height, gravity);

    /* Set up the vertex data, buffers, vertex attributes etc */
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set the projection matrix as ortho
    glm::mat4 projection = glm::ortho(-win_width/2.0f, win_width/2.0f, -win_height/2.0f, win_height/2.0f, -1.0f, 10.0f);
    unsigned int prjLoc = glGetUniformLocation(ourShader.ID, "projection"); // Doing this here instead of in the main loop

    // Init model and view matrix
    glm::mat4 model         = ball->model_matrix; // obviously it comes from the ball
    glm::mat4 view          = glm::mat4(1.0f);

    int win_pos_x, win_pos_y; // For comparing window positions
    glfwGetWindowPos(window, &win_pos_x, &win_pos_y); // Get the initial window position
    ball->init_window_position(win_pos_x, win_pos_y);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Use the shaders duh
        ourShader.use();

        // Do transformations
        glfwGetWindowPos(window, &win_pos_x, &win_pos_y);
        ball->update_window_velocity(win_pos_x, win_pos_y);
        ball->update(); // Perform any movements

        // Get the model and view matrices
        model = ball->model_matrix;
        view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");

        // pass them to the shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(prjLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, array_size);
        //draw();
        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }

    // Deallocate
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete(ball);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}