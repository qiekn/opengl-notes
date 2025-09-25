#include "test_texture2d.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "renderer.h"
#include "vertex_buffer_layout.h"

namespace test {
TestTexture2D::TestTexture2D()
    : proj_(glm::ortho(0.0f, 960.0f, 0.0f, 720.0f, -1.0f, 1.0f)),
      view_(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
      translation_a_(glm::vec3(200, 200, 0)),
      translation_b_(glm::vec3(400, 200, 0)) {
  float positions[] = {
      100.0f, 100.0f, 0.0f, 0.0f,  // 0
      200.0f, 100.0f, 1.0f, 0.0f,  // 1
      200.0f, 200.0f, 1.0f, 1.0f,  // 2
      100.0f, 200.0f, 0.0f, 1.0f   // 3
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  vao_ = std::make_unique<VertexArray>();

  vertex_buffer_ = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(2);
  vao_->AddBuffer(*vertex_buffer_, layout);

  index_buffer_ = std::make_unique<IndexBuffer>(indices, 6);

  shader_ = std::make_unique<Shader>("assets/shaders/basic.shader");
  shader_->Bind();
  shader_->SetUniform4f("u_color", 0.2f, 0.3f, 0.8f, 1.0f);
  shader_->SetUniform1i("u_texture", 0);

  texture_ = std::make_unique<Texture>("assets/textures/cat.jpg");
}

TestTexture2D::~TestTexture2D() {}

void TestTexture2D::OnUpdate(float deltaTime) {}

void TestTexture2D::OnRender() {
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT));

  Renderer renderer;

  texture_->Bind();

  {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), translation_a_);
    glm::mat4 mvp = proj_ * view_ * model;

    shader_->Bind();
    shader_->SetUniformMat4f("u_mvp", mvp);

    renderer.Draw(*vao_, *index_buffer_, *shader_);
  }

  {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), translation_b_);
    glm::mat4 mvp = proj_ * view_ * model;

    shader_->Bind();
    shader_->SetUniformMat4f("u_mvp", mvp);

    renderer.Draw(*vao_, *index_buffer_, *shader_);
  }
}

void TestTexture2D::OnImGuiRender() {
  ImGui::SliderFloat3("translation_a_", &translation_a_.x, 0.0f, 960.0f);
  ImGui::SliderFloat3("translation_b_", &translation_b_.x, 0.0f, 960.0f);
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
              ImGui::GetIO().Framerate);
}
}  // namespace test
