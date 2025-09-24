// clang-format off
#include "glad/gl.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <cstdlib>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/matrix_transform.hpp"  // IWYU pragma: keep
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "index_buffer.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 600;

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

  GLFWwindow* window = glfwCreateWindow(kScreenWidth, kScreenHeight, "ck::cherno_opengl_tutorial", NULL, NULL);
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
      -50.0f, -50.0f, 0.0f, 0.0f,  // 0
      50.0f,  -50.0f, 1.0f, 0.0f,  // 1
      50.0f,  50.0f,  1.0f, 1.0f,  // 2
      -50.0f, 50.0f,  0.0f, 1.0f   // 3
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

  /*────────────┐
  │ ImGUi Setup │
  └─────────────*/

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 8.0f;
  style.FrameRounding = 8.0f;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  // state
  glm::vec3 translate_a(200, 200, 0);
  glm::vec3 translate_b(400, 200, 0);

  /*───────┐
  │ Shader │
  └────────*/

  glm::mat4 proj = glm::ortho(0.0f, (float)kScreenWidth, 0.0f, (float)kScreenHeight, -1.0f, 1.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

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

    // Start Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
      ImGui::Begin("Positions");
      ImGui::SliderFloat3("Translation A", &translate_a.x, 0.0f, (float)kScreenWidth);
      ImGui::SliderFloat3("Translation B", &translate_b.x, 0.0f, (float)kScreenWidth);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    shader.Bind();
    shader.SetUniform4f("u_color", 1.0, 0.5, color_b, 1.0f);

    va.Bind();
    ib.Bind();

    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), translate_a);
      glm::mat4 mvp = proj * view * model;  // model-view-projection
      shader.SetUniformMat4f("u_mvp", mvp);
      renderer.Draw(va, ib, shader);
    }
    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), translate_b);
      glm::mat4 mvp = proj * view * model;  // model-view-projection
      shader.SetUniformMat4f("u_mvp", mvp);
      renderer.Draw(va, ib, shader);
    }

    // test uniform variable
    if (color_b >= 1.0f)
      increment = -0.05f;
    else if (color_b <= 0)
      increment = 0.05f;
    color_b += increment;

    // Render Dear ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update
    GLCall(glfwSwapBuffers(window));
    GLCall(glfwPollEvents());
  }

  // Cleanup Dear ImGui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}
