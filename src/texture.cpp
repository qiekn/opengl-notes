#include "texture.hpp"
#include <iostream>
#include "glad/gl.h"
#include "stb_image.h"

Texture::Texture(const std::string& path) : id_(0), filepath_(path), local_buffer_(nullptr), width_(0), height_(0) {
  stbi_set_flip_vertically_on_load(1);
  local_buffer_ = stbi_load(path.c_str(), &width_, &height_, &channels_, 4);

  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // Minify
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // Magnify
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer_);
  glBindTexture(GL_TEXTURE_2D, 0);

  if (local_buffer_) {
    stbi_image_free(local_buffer_);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
}

Texture::~Texture() { glDeleteTextures(1, &id_); }

void Texture::Bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
