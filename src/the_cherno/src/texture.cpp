#include "texture.h"
#include "stb_image.h"

Texture::Texture(const std::string& path)
    : renderer_id_(0), file_path_(path), local_buffer_(nullptr), width_(0), height_(0), bpp_(0) {
  stbi_set_flip_vertically_on_load(1);
  local_buffer_ = stbi_load(path.c_str(), &width_, &height_, &bpp_, 4);

  GLCall(glGenTextures(1, &renderer_id_));
  GLCall(glBindTexture(GL_TEXTURE_2D, renderer_id_));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                      local_buffer_));
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));

  if (local_buffer_) {
    stbi_image_free(local_buffer_);
  }
}

Texture::~Texture() { GLCall(glDeleteTextures(1, &renderer_id_)); }

void Texture::Bind(unsigned int slot) const {
  GLCall(glActiveTexture(GL_TEXTURE0 + slot));
  GLCall(glBindTexture(GL_TEXTURE_2D, renderer_id_));
}

void Texture::Unbind() { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }
