#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "index_buffer.h"
#include "shader.h"
#include "test.h"
#include "texture.h"
#include "vertex_array.h"

namespace test {
class TestTexture2D : public Test {
public:
  TestTexture2D();
  virtual ~TestTexture2D();

  void OnUpdate(float deltaTime) override;
  void OnRender() override;
  void OnImGuiRender() override;

private:
  std::unique_ptr<VertexArray> vao_;
  std::unique_ptr<IndexBuffer> index_buffer_;
  std::unique_ptr<VertexBuffer> vertex_buffer_;
  std::unique_ptr<Shader> shader_;
  std::unique_ptr<Texture> texture_;

  glm::mat4 proj_, view_;
  glm::vec3 translation_a_, translation_b_;
};
}  // namespace test
