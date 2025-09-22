#pragma once

class VertexBuffer {
public:
  VertexBuffer(const void* data, unsigned int size);
  virtual ~VertexBuffer();

  void Bind() const;
  void Unbind() const;

private:
  unsigned renderer_id_;
};
