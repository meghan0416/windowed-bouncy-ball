/*

Author: Meghan Grayson
Last Modified Date: December 1, 2024

*/

#include "Circle.hpp"

/* Globals and temps */
float t0_x, t0_y;                           // The last time the ball bounced, or the start time for projectile motion
glm::vec2 last_window_pos;                  // The last position of the window recorded
float last_window_time;                     // The last time the position was recorded

// Basically the same but use given initial_position and radius
Circle::Circle(int rad, glm::vec2 pos, int w, int h, bool grav) {
    initial_position = pos;
    radius = rad;

    // Set up the window stuff for collisions
    window_width = w;
    window_height = h;
    max_width = (float)w/2.0f;
    min_width = -max_width;
    max_height = (float)h/2.0f;
    min_height = -max_height;

    // Turn on or off gravity
    if(grav) gravity = glm::vec2(0.0f, -2.5f);
    else gravity = glm::vec2(0.0f);

    init();
}

void Circle::init() {
    model_matrix = glm::mat4(1.0f);
    relative_translate = glm::vec3(0.0f);
    initial_velocity = glm::vec2(0.0f);
    window_velocity = glm::vec2(0.0f);
    velocity = glm::vec2(0.0f);
    last_window_pos = glm::vec2(0, 0); // dummy values
    t0_x = glfwGetTime(); // init
    t0_y = glfwGetTime();
    last_window_time = glfwGetTime();
}

// Called by the main loop every frame
void Circle::update() {
    move(); // Update the relative translate in x and y
    // update the model matrix
    model_matrix = glm::translate(glm::mat4(1.0f), relative_translate); // since it is a circle just translate
}

void Circle::move() {
    float scale_factor = 30.f;
    float current_time = glfwGetTime();
    float tx = (current_time - t0_x) * scale_factor; // Use this as t
    float ty = (current_time - t0_y) * scale_factor;

    // Use formulae for projectile motion
    velocity.y = (initial_velocity.y + (gravity.y * ty)); // Update Y velocity
    relative_translate.y = initial_position.y + (0.5f * (initial_velocity.y + velocity.y) * ty); // Update Y position
    relative_translate.x = initial_position.x + (velocity.x * tx);

    // Check for collisions
    // Only if window size has been set
    float damping = 0.7f; // Adjust as needed
    if(collideX(damping)) initProjectileX();
    if(collideY(damping)) initProjectileY();

}

// Detect collisions and correct the position if needed
// Collisions also dampen the velocity a little
bool Circle::collideX(float d) {
    if(relative_translate.x + radius > max_width) {
        relative_translate.x = max_width-radius;
        velocity.x *= -d; // apply the dampening factor
        velocity.x += window_velocity.x; // Add the window's impact if it was moving
        return true;
    }
    else if (relative_translate.x - radius < min_width) {
        relative_translate.x = min_width+radius;
        velocity.x *= -d; // apply the dampening factor
        velocity.x += window_velocity.x; // Add the window's impact if it was moving
        return true;
    }
    else return false;
}

// Detect collision and correct the position if needed
// Adjusted to behave differently for ground collisions
bool Circle::collideY(float d) {
    if(relative_translate.y + radius > max_height) {
        relative_translate.y = max_height-radius;
        velocity.y *= -1; // don't dampen collisions at the top
        velocity.y -= window_velocity.y; // Add the window's impact if it was moving
        initial_velocity.y = velocity.y; 
        return true;
    }
    else if (relative_translate.y - radius < min_height) {
        relative_translate.y = min_height+radius;
        velocity.y *= -d; // apply the dampening factor
        velocity.y -= window_velocity.y; // Add the window's impact if it was moving
        initial_velocity.y = velocity.y;
        return true;
    }
    else return false;
}

// Provided x and y is the new position of the window
// Compute the window's velocity with the time and new position
void Circle::update_window_velocity(int x, int y) {
    // Compute the change in window position
    glm::vec2 delta_position = glm::vec2(x - last_window_pos.x, y - last_window_pos.y);

    // Update the initial position of the ball with these values
    // Smoothed by averaging the old initial position with the new one, times a scale factor
    initial_position.y = 0.5 * (initial_position.y + (initial_position.y + delta_position.y*0.5));
    initial_position.x = 0.5 * (initial_position.x + (initial_position.x - delta_position.x*0.5));

    // Update the window's velocity by computing d/t
    float scale_factor = 0.003f;
    float time = glfwGetTime();
    float deltatime = time-last_window_time;
    window_velocity = glm::vec2((float)(x - last_window_pos.x)/deltatime * scale_factor, (float)(y - last_window_pos.y)/deltatime * scale_factor);

    // Update the last position and last time
    last_window_time = time;
    last_window_pos = glm::vec2(x, y);
}


void Circle::initProjectileY() {
    t0_y = glfwGetTime();
    initial_position.y = relative_translate.y;
}

void Circle::initProjectileX() {
    t0_x = glfwGetTime();
    initial_position.x = relative_translate.x;
}

// Set the time and position of the window at the start of the program
void Circle::init_window_position(int x, int y) {
    last_window_pos = glm::vec2(x, y);
    last_window_time = glfwGetTime();
}