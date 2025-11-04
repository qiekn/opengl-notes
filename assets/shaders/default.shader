#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;

out vec3 vPosition;

void main() {
  vPosition = aPosition;
  gl_Position = vec4(aPosition, 1.0);
}

#shader fragment

#version 330 core
layout(location = 0) out vec4 color;

in vec3 vPosition;

void main() {
  color = vec4(vPosition * 0.5 + 0.5, 1.0);
}

// vim: set ft=glsl
