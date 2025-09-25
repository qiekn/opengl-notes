#include "test_batch_render.h"

#include "imgui.h"
#include "renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "vertex_buffer_layout.h"

namespace test {
TestBatchRender::TestBatchRender()
    : proj_(glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f)),
      view_(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
      translation_(glm::vec3(0, 0, 0)) {
  float positions[] = {
      100.0f, 100.0f, 200.0f, 100.0f, 200.0f, 200.0f, 100.0f, 200.0f,

      300.0f, 100.0f, 400.0f, 100.0f, 400.0f, 200.0f, 300.0f, 200.0f,

  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4};

  vao_ = std::make_unique<VertexArray>();

  vertex_buffer_ = std::make_unique<VertexBuffer>(positions, 8 * 2 * sizeof(float));
  VertexBufferLayout layout;
  layout.Push<float>(2);
  vao_->AddBuffer(*vertex_buffer_, layout);

  index_buffer_ = std::make_unique<IndexBuffer>(indices, 12);

  shader_ = std::make_unique<Shader>("assets/shaders/batch.shader");
  shader_->Bind();
}

TestBatchRender::~TestBatchRender() {}

void TestBatchRender::OnUpdate(float deltaTime) {}

void TestBatchRender::OnRender() {
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT));

  Renderer renderer;
  glm::mat4 mvp = proj_ * view_;

  shader_->Bind();
  shader_->SetUniformMat4f("u_mvp", mvp);

  renderer.Draw(*vao_, *index_buffer_, *shader_);
}

void TestBatchRender::OnImGuiRender() {
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
              ImGui::GetIO().Framerate);
}
}  // namespace test
