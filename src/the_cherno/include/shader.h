#pragma once

#include <string>
#include <unordered_map>
#include "glm/fwd.hpp"

struct ShaderProgramSource {
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
  void SetUniform1f(const std::string& name, float value);
  void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
  void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
  ShaderProgramSource ParseShader(const std::string& filepath);
  unsigned int CompileShader(unsigned int type, const std::string& source);
  unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
  int GetUniformLocation(const std::string& name) const;

private:
  unsigned int renderer_id_;
  std::string file_path_;
  mutable std::unordered_map<std::string, int> uniform_location_cache_;
};
