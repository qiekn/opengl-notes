#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec4 vColor;
out vec2 vTexCoord;

void main() {
  vColor = vec4(aPosition * 0.5 + 0.5, 1.0);
  vTexCoord = aTexCoord;
  gl_Position = vec4(aPosition, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoord;

uniform sampler2D Texture;

void main() {
  FragColor = texture(Texture, vTexCoord);
}

// vim: set ft=glsl
