#pragma once

#include "renderer.h"  // IWYU pragma: keep

class Texture {
public:
  Texture(const std::string& path);
  ~Texture();

  void Bind(unsigned int slot = 0) const;
  void Unbind();

  inline int GetWidth() const { return width_; }
  inline int GetHeight() const { return height_; }

private:
  unsigned int renderer_id_;
  std::string file_path_;
  unsigned char* local_buffer_;
  int width_, height_, bpp_;
};
