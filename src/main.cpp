// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h> // GLFW (include after glad)
// clang-format on

#include <cstdio>
#include <string>

// Shader code
std::string vertex_source = R"(
  #version 330 core

  layout(location = 0) in vec3 aPosition;

  out vec3 vPosition;

  void main() {
    vPosition = aPosition;
    gl_Position = vec4(aPosition, 1.0);
  }
)";

std::string fragment_source = R"(
  #version 330 core

  layout(location = 0) out vec4 color;

  in vec3 vPosition;

  void main() {
    color = vec4(vPosition * 0.5 + 0.5, 1.0);
  }
)";

int main(void) {
  // Initialize GLFW library
  if (!glfwInit()) return -1;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Create a windowed mode window and its OpenGL context
  GLFWwindow* window = glfwCreateWindow(640, 480, "ck: my first triangle", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // GLAD
  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    printf("Failed to initialize OpenGL context\n");
    return -1;
  }

  // Successfully loaded OpenGL
  printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

  // clang-format off
  // Triangle vertices (after MVP transform)
  float vertices[] = {
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.0f,  0.5f, 0.0f
  };
  // clang-format on

  unsigned int VAO;  // Vertex Array Object
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;  // Vertex Buffer Object
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);  // Unbind VAO after configuring all the vertex attribute pointers and buffers,
                         // Unbinding the VAO prevents accidental modification by subsequent OpenGL calls.

  // Compile shaders
  // 1. Vertex shader
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  auto source = (const GLchar*)vertex_source.c_str();
  glShaderSource(vertex_shader, 1, &source, NULL);
  glCompileShader(vertex_shader);
  // 2. Fragment shader
  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  source = (const GLchar*)fragment_source.c_str();
  glShaderSource(fragment_shader, 1, &source, NULL);
  glCompileShader(fragment_shader);

  // Shader Program
  unsigned int shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Render here
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  // Cleanup OpenGL resources
  glDeleteProgram(shader_program);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}
