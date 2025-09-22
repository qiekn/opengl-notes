#pragma once

class IndexBuffer {
public:
  IndexBuffer(const unsigned int* data, unsigned int count);
  virtual ~IndexBuffer();

  void Bind() const;
  void Unbind() const;

private:
  unsigned int renderer_id_;
  unsigned int count_;
};
