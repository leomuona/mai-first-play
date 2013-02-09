#include "shader.hpp"

#include <GL/glew.h>

#include <string>
#include <cstdio>
#include <vector>
#include <fstream>
#include <algorithm>

namespace shader {

    GLuint LoadShaders(const char *vertex_file_path,
            const char *fragment_file_path)
    {
        // create the shaders
        GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        // read the vertex shader code from file
        std::string vertexShaderCode;
        std::ifstream vertexShaderStream(vertex_file_path, std::ios::in);
        if (vertexShaderStream.is_open()) {
            std::string line = "";
            while (getline(vertexShaderStream, line)) {
                vertexShaderCode += "\n" + line;
            }
            vertexShaderStream.close();
        }

        // read the fragment shader code from file
        std::string fragmentShaderCode;
        std::ifstream fragmentShaderStream(fragment_file_path, std::ios::in);
        if (fragmentShaderStream.is_open()) {
            std::string line = "";
            while (getline(fragmentShaderStream, line)) {
                fragmentShaderCode += "\n" + line;
            }
            fragmentShaderStream.close();
        }

        GLint result = GL_FALSE;
        int infoLogLength;

        // compile vertex shader
        printf("Compiling shader : %s\n", vertex_file_path);
        char const *vertexSourcePointer = vertexShaderCode.c_str();
        glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
        glCompileShader(vertexShaderID);

        // check vertex shader
        glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> vertexShaderErrorMessage(infoLogLength);
        glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL,
                &vertexShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &vertexShaderErrorMessage[0]);

        // compile fragment shader
        printf("Compiling shader : %s\n", fragment_file_path);
        char const *fragmentSourcePointer = fragmentShaderCode.c_str();
        glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
        glCompileShader(fragmentShaderID);

        // check fragment shader
        glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> fragmentShaderErrorMessage(infoLogLength);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL,
                &fragmentShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &fragmentShaderErrorMessage[0]);

        // link the program
        fprintf(stdout, "Linking program\n");
        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);

        // check the program
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> programErrorMessage(std::max(infoLogLength, int(1)));
        glGetProgramInfoLog(programID, infoLogLength, NULL,
                &programErrorMessage[0]);
        fprintf(stdout, "%s\n", &programErrorMessage[0]);

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        return programID;
    }

}; // namespace shader
