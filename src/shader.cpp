#include "shader.hpp"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include "glad/gl.h"

Shader::Shader(const std::string& filepath) : filepath_(filepath) {
  ShaderSource source = Parse(filepath);
  id_ = Create(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() { glDeleteProgram(id_); }

void Shader::Bind() const { glUseProgram(id_); }

void Shader::Unbind() const { glUseProgram(0); }

void Shader::SetUniform1i(const std::string& name, int value) { glUniform1i(GetUniformLocation(name), value); }

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
  glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMatrix4fv(const std::string& name, glm::mat4 trans) {
  glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(trans));
}

ShaderSource Shader::Parse(const std::string& filepath) {
  std::ifstream stream(filepath);
  enum class ShaderType { kNone = -1, kVertex = 0, kFragment = 1 };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::kNone;
  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        // set mode to vertex
        type = ShaderType::kVertex;
      } else if (line.find("fragment") != std::string::npos) {
        // set mode to fragment
        type = ShaderType::kFragment;
      }
    } else {
      ss[(int)type] << line << '\n';
    }
  }
  return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::Compile(unsigned int type, const std::string& source) {
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  // Error handling
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile " << (type == (unsigned int)GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
    std::cout << message << std::endl;
    return 0;
  }
  return id;
}

unsigned int Shader::Create(const std::string& vertex_shader, const std::string& fragment_shader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = Compile(GL_VERTEX_SHADER, vertex_shader);
  unsigned int fs = Compile(GL_FRAGMENT_SHADER, fragment_shader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);
  return program;
}

int Shader::GetUniformLocation(const std::string& name) {
  if (uniform_location_cache_.find(name) != uniform_location_cache_.end()) {
    return uniform_location_cache_[name];
  }
  int location;
  location = glGetUniformLocation(id_, name.c_str());
  if (location == -1) {
    std::cout << "Warning: uniform '" << name << "' doesn't exits" << std::endl;
  }
  uniform_location_cache_[name] = location;
  return location;
}
