// clang-format off
#include <alloca.h>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#if defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK() __builtin_trap()
#endif

#define ASSERT(x)                              \
  if (!(x)) {                                  \
    std::cout << "Assert Failed" << std::endl; \
    DEBUG_BREAK();                             \
  }

#define GLCall(x)                              \
  do {                                         \
    GLClearError();                            \
    x;                                         \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)); \
  } while (0);

static void GLClearError() { while (glGetError() != GL_NO_ERROR); }

static bool GLLogCall(const char* function, const char* file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line
              << std::endl;
    return false;
  }
  return true;
}

struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
  std::ifstream stream(filepath);

  enum class ShaderType { kNone = -1, kVertex = 0, kFragment = 1 };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::kNone;
  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        // set mode to vertex
        type = ShaderType::kVertex;
      } else if (line.find("fragment") != std::string::npos) {
        // set mode to fragment
        type = ShaderType::kFragment;
      }
    } else {
      ss[(int)type] << line << '\n';
    }
  }
  return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
  unsigned id;
  GLCall(id = glCreateShader(type));
  const char* src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

  // Error handling
  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE) {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* message = (char*)alloca(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "Failed to compile "
              << (type == (unsigned int)GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
    std::cout << message << std::endl;
    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

static unsigned int CreateShader(const std::string& vertex_shader,
                                 const std::string& fragment_shader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));
  return program;
}

int main() {
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

  GLFWwindow* window = glfwCreateWindow(800, 600, "ck::cherno_opengl_tutorial", NULL, NULL);
  if (!window) {
    printf("Glfw: Failed to create window\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

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
      -0.5f, -0.5f,  // 0
      0.5f,  -0.5f,  // 1
      0.5f,  0.5f,   // 2
      -0.5f, 0.5f,   // 3
  };

  unsigned int indices[]{0, 1, 2, 2, 3, 0};

  unsigned int VAO;                    // Save Vertex Array Object Id
  GLCall(glGenVertexArrays(1, &VAO));  // Generate
  GLCall(glBindVertexArray(VAO));      // Bind
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure
  // vertex attributes(s).

  unsigned int VBO;  // (Vertex Buffer Object, VBO)
  GLCall(glGenBuffers(1, &VBO));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
  GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));
  glEnableVertexAttribArray(0);

  unsigned int IBO;  // (Index Buffer Object, IBO)
  GLCall(glGenBuffers(1, &IBO));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

  // Unbind
  GLCall(glBindVertexArray(0));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

  /*─────────────┐
  │ Apply Shader │
  └──────────────*/

  ShaderProgramSource source = ParseShader("assets/shaders/basic.shader");
  unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
  GLCall(glUseProgram(shader));

  float color_b = 1.0f;
  float increment = -0.05f;

  int location;
  GLCall(location = glGetUniformLocation(shader, "u_color"));
  ASSERT(location != -1);

  /*──────────┐
  │ Main Loop │
  └───────────*/

  while (!glfwWindowShouldClose(window)) {
    // Render here
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    GLCall(glUseProgram(shader));
    GLCall(glUniform4f(location, 1.0, 0.5, color_b, 1.0f));

    GLCall(glBindVertexArray(VAO));
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    // test uniform variable
    if (color_b >= 1.0f)
      increment = -0.05f;
    else if (color_b <= 0)
      increment = 0.05f;
    color_b += increment;

    // Update
    GLCall(glfwSwapBuffers(window));
    GLCall(glfwPollEvents());
  }

  glfwTerminate();
  return 0;
}
