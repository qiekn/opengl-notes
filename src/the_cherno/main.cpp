// clang-format off
#include <alloca.h>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <string>
// clang-format on

static unsigned int CompileShader(unsigned int type, const std::string& source) {
  unsigned id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  // Error handling
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile "
              << (type == (unsigned int)GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int CreateShader(const std::string& vertex_shader,
                                 const std::string& fragment_shader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);
  return program;
}

int main() {
  GLFWwindow* window;

  /*──────────────────────────────────────────────────────┐
  │ GLFW: Initialize and create window and opengl context │
  └───────────────────────────────────────────────────────*/
  if (!glfwInit()) return -1;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(800, 600, "ck::cherno_opengl_tutorial", NULL, NULL);
  if (!window) {
    printf("Glfw: Failed to create window\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  /*────────────────────────────────────────┐
  │ Glad: load all opengl function pointers │
  └─────────────────────────────────────────*/
  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    printf("Glad: Failed to initialize OpenGL context\n");
    return -1;
  }
  printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

  /*──────────────┐
  │ Vertex buffer │
  └───────────────*/

  float vertices[] = {
      -0.5f, -0.5f,  // left
      0.5f,  -0.5f,  // right
      0.0f,  0.5f,   // top
  };

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int buf;  // (Vertex Buffer Object, VBO)
  glGenBuffers(1, &buf);
  glBindBuffer(GL_ARRAY_BUFFER, buf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
  glEnableVertexAttribArray(0);

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  /*─────────────┐
  │ Apply Shader │
  └──────────────*/

  std::string vertex_shader =
      "#version 330 core\n"
      "layout(location = 0) in vec4 position;\n"
      "void main() {\n"
      "  gl_Position = vec4(position.xy, 0.0, 1.0);\n"
      "}\n";

  std::string fragment_shader =
      "#version 330 core\n"
      "out vec4 color;\n"
      "void main() {\n"
      "  color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\n";

  unsigned int shader = CreateShader(vertex_shader, fragment_shader);
  glUseProgram(shader);

  /*──────────┐
  │ Main Loop │
  └───────────*/

  while (!glfwWindowShouldClose(window)) {
    // Render here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Update
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
