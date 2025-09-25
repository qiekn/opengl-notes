#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

// vim: set ft=glsl
