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
      100.0f, 100.0f, 0.0f, 0.0f,  // 0
      200.0f, 100.0f, 1.0f, 0.0f,  // 1
      200.0f, 200.0f, 1.0f, 1.0f,  // 2
      100.0f, 200.0f, 0.0f, 1.0f   // 3
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

  glm::mat4 proj = glm::ortho(0.0f, (float)kScreenWidth, 0.0f, (float)kScreenHeight, -1.0f, 1.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
  glm::mat4 mvp = proj * view * model;  // model-view-projection

  Shader shader("assets/shaders/basic.shader");
  shader.Bind();
  shader.SetUniform4f("u_color", 1.0f, 0.5f, 0.2f, 1.0f);
  shader.SetUniformMat4f("u_mvp", mvp);

  Texture texture("assets/textures/cat.jpg");
  texture.Bind();
  shader.SetUniform1i("u_texture", 0);

  float color_b = 1.0f;
  float increment = -0.05f;

  /*────────────┐
  │ ImGUi Setup │
  └─────────────*/

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  // state
  bool show_demo_window = false;
  bool show_another_window = true;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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

    // 1. Show the big demo window
    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves.
    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Begin("Hello, world!");

      ImGui::Text("This is some useful text.");
      ImGui::Checkbox("Demo Window", &show_demo_window);
      ImGui::Checkbox("Another Window", &show_another_window);

      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3("clear color", (float*)&clear_color);  // Edit 3 floats representing a color

      if (ImGui::Button("Button"))  // Buttons return true when clicked
        counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window) {
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me")) show_another_window = false;
      ImGui::End();
    }

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
