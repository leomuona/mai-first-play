#include "shader.hpp"

#include <GL/glew.h>
#include <GL/glfw.h> // handles window + keyboard
#include <glm/glm.hpp> // for vec3
#include <glm/gtc/matrix_transform.hpp> // matrix fun

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

void drawTriangle(GLuint &vertexbuffer)
{
    // first attribute buffer: vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,          // attribute 0.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
    );

    // draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
}

glm::mat4 moveCamera(int x, int y, int z)
{
    // Projection matrix
    // 45 degree field of view, 4:3 aspect ratio, 0.1 - 100.0 display range
    glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // camera matrix
    glm::mat4 view = glm::lookAt(
        glm::vec3(x,y,z), // camera position
        glm::vec3(0,0,0), // look at origo
        glm::vec3(0,1,0) // up-vector
    );
    // model matrix: an identity matrix (model will be at origo)
    glm::mat4 model = glm::mat4(1.0f); // changes for each model
    // our model view projection : multiplication of 3 matrices
    // does model -> view -> projection
    glm::mat4 mvp = projection * view * model; 
    return mvp;    
}

inline long getTimeInMillis()
{
    return static_cast<long> (glfwGetTime() * 1000);
}

void mainLoop(GLuint &vertexbuffer, GLuint &glslProgramID,
        GLuint &matrixID, glm::mat4 &mvp)
{
    long start, end;
    start = getTimeInMillis();
    end = getTimeInMillis();
    bool running = true;
    while (running) {
        // limited to 60 fps 
        if (end - start > 16) {
            start = getTimeInMillis();
            
            // clear the screen
            glClear(GL_COLOR_BUFFER_BIT);

            // use our shaders
            glUseProgram(glslProgramID);

            // send our transofrmation to currently bound shader,
            // in the "MVP" uniform
            // for each model that is rendered, since the MVP will be different
            // (atleast model part)
            glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]); 

            drawTriangle(vertexbuffer);

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
        glfwTerminate(); 
        return -1;
    }

    glfwEnable(GLFW_STICKY_KEYS);

    // blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // create and bind vertex array
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID); 
    
    // create and compile GLSL program from shaders
    GLuint glslProgramID = shader::LoadShaders(
            "shaders/simple.vert.glsl",
            "shaders/simple.frag.glsl");
 
    // get a handle for our "MVP" uniform
    // only at initialisation time.
    GLuint matrixID = glGetUniformLocation(glslProgramID, "MVP"); 
    
    // move camera
    glm::mat4 mvp = moveCamera(4,3,3);
     
    // define 3 vectors to represent 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    // create vertex buffer and pass it to OpenGL
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
            g_vertex_buffer_data, GL_STATIC_DRAW);
    
    // do the main loop
    mainLoop(vertexbuffer, glslProgramID, matrixID, mvp);

    glfwTerminate();

    // cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &vertexArrayID);

    return 0;
}
