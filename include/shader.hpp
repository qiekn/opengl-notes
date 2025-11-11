#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderSource {
  std::string VertexSource;
  std::string FragmentSource;
};

class Shader {
public:
  Shader(const std::string& filepath);
  ~Shader();

  void Bind() const;
  void Unbind() const;
  void SetUniform1i(const std::string& name, int value);
  void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
  void SetUniformMatrix4fv(const std::string& name, glm::mat4 trans);

private:
  ShaderSource Parse(const std::string& filepath);
  unsigned int Compile(unsigned int type, const std::string& source);
  unsigned int Create(const std::string& vertex_shader, const std::string& fragment_shader);
  int GetUniformLocation(const std::string& name);

private:
  unsigned int id_;
  std::string filepath_;
  std::unordered_map<std::string, int> uniform_location_cache_;
};
