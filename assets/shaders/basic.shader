#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textcoord;

out vec2 v_textcoord;

void main() {
    gl_Position = position;
    v_textcoord = textcoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_textcoord;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main() {
    vec4 textcolor = texture(u_texture, v_textcoord);
    color = textcolor * u_color;
}

// vim: ft=glsl
