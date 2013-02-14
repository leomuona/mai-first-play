#version 330 core

// input vertex data
layout(location = 0) in vec3 vPosition;
uniform mat4 MVP;

void main()
{
    // output position of the vertex, in clip space: MVP * position

    vec4 v = vec4(vPosition, 1); // transform a homogeneous 4D vector
    gl_Position = MVP * v;
}
