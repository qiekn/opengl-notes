// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h> // GLFW (include after glad)
// clang-format on

#include <cstdio>
#include <iostream>
#include <string>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/glm.hpp"
#include "shader.hpp"
#include "texture.hpp"

// ImGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

  // ImGui Setup
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 8.0f;
  style.FrameRounding = 8.0f;
  style.FontScaleDpi = 2.0f;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  bool show_demo_window = false;

  // Transformation parameters
  glm::vec3 translation(0.5f, -0.5f, 0.0f);
  float rotation_angle = 0.0f;
  bool auto_rotate = true;
  glm::vec3 scale(1.0f, 1.0f, 1.0f);

  // clang-format off
  // Triangle vertices (after MVP transform)
  float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
  };
  // clang-format on

  unsigned int indices[] = {
      0, 1, 3,  // the 1st triangle
      1, 2, 3   // the 2nd triangle
  };

  unsigned int VAO;  // Vertex Array Object
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;  // Vertex Buffer Object
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  unsigned int IBO;  // Index Buffer Object (or Element Buffer Object, EBO)
  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);  // Unbind VAO after configuring all the vertex attribute pointers and buffers,
  // Unbinding the VAO prevents accidental modification by subsequent OpenGL calls.

  Shader shader("assets/shaders/default.shader");
  Texture texture("assets/textures/cat.jpg");

  shader.Bind();
  shader.SetUniform1i("Texture", 0);

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Render here
    glClear(GL_COLOR_BUFFER_BIT);

    // ImGui Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // Show a simple debug window
    {
      if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
      ImGui::Begin("Debug");
      ImGui::Text("Hello from ImGui");
      ImGui::Checkbox("Show Demo Window", &show_demo_window);

      ImGui::Separator();
      ImGui::Text("Transformations");
      ImGui::SliderFloat3("Translation", &translation.x, -1.0f, 1.0f);
      ImGui::SliderFloat3("Scale", &scale.x, 0.1f, 3.0f);
      ImGui::Checkbox("Auto Rotate", &auto_rotate);
      if (!auto_rotate) {
        ImGui::SliderAngle("Rotation", &rotation_angle, 0.0f, 360.0f);
      }

      ImGui::End();
    }

    shader.Bind();
    texture.Bind();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Transformations
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, translation);
    float angle = auto_rotate ? (float)glfwGetTime() : rotation_angle;
    trans = glm::rotate(trans, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, scale);
    shader.SetUniformMatrix4fv("transform", trans);

    // Render Dear ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  // Cleanup OpenGL resources
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  // Cleanup Dear ImGui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}
