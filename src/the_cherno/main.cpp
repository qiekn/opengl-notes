// clang-format off
#include "glad/gl.h"
#include "GLFW/glfw3.h"
// clang-format on
#include <cstdlib>
#include "index_buffer.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

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
      -0.5f, -0.5f, 0.0f, 0.0f,  // 0
      0.5f,  -0.5f, 1.0f, 0.0f,  // 1
      0.5f,  0.5f,  1.0f, 1.0f,  // 2
      -0.5f, 0.5f,  0.0f, 1.0f   // 3
  };

  unsigned int indices[]{0, 1, 2, 2, 3, 0};

  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  VertexArray va;
  VertexBuffer vb(position, 4 * 4 * sizeof(float));

  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);

  IndexBuffer ib(indices, 6);

  // Unbind
  vb.Unbind();
  // ib.Unbind();

  Renderer renderer;

  /*───────┐
  │ Shader │
  └────────*/

  Shader shader("assets/shaders/basic.shader");
  shader.Bind();
  shader.SetUniform4f("u_color", 1.0f, 0.5f, 0.2f, 1.0f);

  Texture texture("assets/textures/cat.jpg");
  texture.Bind();
  shader.SetUniform1i("u_texture", 0);

  float color_b = 1.0f;
  float increment = -0.05f;

  /*──────────┐
  │ Main Loop │
  └───────────*/

  while (!glfwWindowShouldClose(window)) {
    // Render here
    renderer.Clear();

    shader.Bind();
    shader.SetUniform4f("u_color", 1.0, 0.5, color_b, 1.0f);

    va.Bind();
    ib.Bind();

    renderer.Draw(va, ib, shader);

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
