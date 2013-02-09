#version 330 core

// input vertex data
layout(location = 0) in vec3 vPosition;

void main()
{
    gl_Position.xyz = vPosition;
    gl_Position.w = 1.0;
}
