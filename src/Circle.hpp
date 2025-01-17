/*

Author: Meghan Grayson
Last Modified Date: December 1, 2024

*/


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Circle {
    private:
        int radius;
        glm::vec2 initial_position;
        glm::vec3 relative_translate;
        glm::vec2 initial_velocity;
        glm::vec2 gravity;
        glm::vec2 velocity; // Speed and direction per frame
        glm::vec2 window_velocity; // Speed and direction of the window itself

        // For computing collisions
        int window_height;
        int window_width;
        int max_width;
        int max_height;
        int min_width;
        int min_height;

        void init();
        void move();
        bool collideX(float d);
        bool collideY(float d);
        void initProjectileY(); // Use these when there is a collision
        void initProjectileX();

    public:
        glm::mat4 model_matrix;

        Circle(int rad, glm::vec2 pos, int w, int h, bool grav); // radius, initial position, width and height of window, boolean for gravity on or off
        void init_window_position(int x, int y);

        void update();
        void update_window_velocity(int x, int y); // the most important mechanic, pass the new coordinates of the window
};
