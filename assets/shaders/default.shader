#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
  vTexCoord = aTexCoord;
  gl_Position = proj * view * model * vec4(aPosition, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D Texture;

void main() {
  FragColor = texture(Texture, vTexCoord);
}

// vim: set ft=glsl
