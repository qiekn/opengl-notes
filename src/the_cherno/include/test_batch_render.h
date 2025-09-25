#pragma once

#include "index_buffer.h"
#include "shader.h"
#include "test.h"

#include "glm/glm.hpp"
#include "vertex_array.h"

#include <memory>

namespace test {

class TestBatchRender : public Test {
public:
  TestBatchRender();
  ~TestBatchRender();

  void OnUpdate(float deltaTime) override;
  void OnRender() override;
  void OnImGuiRender() override;

private:
  std::unique_ptr<VertexArray> vao_;
  std::unique_ptr<IndexBuffer> index_buffer_;
  std::unique_ptr<VertexBuffer> vertex_buffer_;
  std::unique_ptr<Shader> shader_;

  glm::mat4 proj_, view_;
  glm::vec3 translation_;
};
}  // namespace test
