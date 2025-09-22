#include "vertex_array.h"
#include <cstdint>
#include "renderer.h"
#include "vertex_buffer_layout.h"

VertexArray::VertexArray() { GLCall(glGenVertexArrays(1, &renderer_id_)); }

VertexArray::~VertexArray() { GLCall(glDeleteVertexArrays(1, &renderer_id_)); }

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
  Bind();
  vb.Bind();
  // Tell GPU buffer layout
  const auto& elements = layout.GetElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    const auto& e = elements[i];

    GLCall(glVertexAttribPointer(i, e.count, e.type, e.normalized, layout.GetStride(),
                                 (const void*)(uintptr_t)offset));
    GLCall(glEnableVertexAttribArray(i));
    offset += e.count * VertexBufferElement::GetSizeOfType(e.type);
  }
}

void VertexArray::Bind() const { GLCall(glBindVertexArray(renderer_id_)); }

void VertexArray::Unbind() const { GLCall(glBindVertexArray(0)); }
