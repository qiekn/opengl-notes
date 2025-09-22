#pragma once

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class VertexArray {
public:
  VertexArray();
  virtual ~VertexArray();

  void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

  void Bind() const;
  void Unbind() const;

private:
  unsigned int renderer_id_;
};
