// clang-format off
#include "glad/gl.h"
#include "GLFW/glfw3.h"
// clang-format on
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "index_buffer.h"
#include "renderer.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

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

  /*───────┐
  │ Vertex │
  └────────*/
  float position[] = {
      -0.5f, -0.5f,  // 0
      0.5f,  -0.5f,  // 1
      0.5f,  0.5f,   // 2
      -0.5f, 0.5f,   // 3
  };

  unsigned int indices[]{0, 1, 2, 2, 3, 0};

  VertexArray va;
  VertexBuffer vb(position, 4 * 2 * sizeof(float));

  VertexBufferLayout layout;
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);

  IndexBuffer ib(indices, 6);

  // Unbind
  vb.Unbind();
  // ib.Unbind();

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

    va.Bind();
    ib.Bind();

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

  GLCall(glDeleteProgram(shader));
  glfwTerminate();
  return 0;
}
