#include <GL/glew.h>
#include <GL/glfw.h> // handles window + keyboard
#include <glm/glm.hpp> // for vec3

#include <cstdio>
#include <cstdlib>
#include <string>

int initWindow(int width, int height, const std::string &title)
{
    // init GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize FLGW\n");
        return -1;
    }

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialising
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3); // use OpenGL 3.3
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwOpenWindow(width, height, 0,0,0,0, 32,0, GLFW_WINDOW)) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // init GLEW
    glewExperimental = true; // needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glfwSetWindowTitle(title.c_str());

    return 0;
}

inline long getTimeInMillis()
{
    return static_cast<long> (glfwGetTime() * 1000);
}

void mainLoop()
{
    glfwEnable(GLFW_STICKY_KEYS);
    
    long start, end;
    start = getTimeInMillis();
    end = getTimeInMillis();
    bool running = true;
    while (running) {
        // limited to 60 fps 
        if (end - start > 16) {
            start = getTimeInMillis();

            glfwSwapBuffers(); // swap buffers
        
            if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS ||
                !glfwGetWindowParam(GLFW_OPENED)) {
                running = false;
            }
        }
        end = getTimeInMillis();
    }
}

int main()
{
    if (initWindow(1024, 768, "Mai First Play") != 0) {
        fprintf(stderr, "Error occurred, exiting main program\n");
        return -1;
    }

    mainLoop();

    return 0;
}
