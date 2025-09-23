#include "shader.h"
#include <fstream>
#include <sstream>
#include "renderer.h"

Shader::Shader(const std::string& filepath) : file_path_(filepath) {
  ShaderProgramSource source = ParseShader(filepath);
  renderer_id_ = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() { GLCall(glDeleteProgram(renderer_id_)); }

void Shader::Bind() const { GLCall(glUseProgram(renderer_id_)); }

void Shader::Unbind() const { GLCall(glUseProgram(0)); }

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
  GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1i(const std::string& name, int value) {
  GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
  GLCall(glUniform1f(GetUniformLocation(name), value));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
  unsigned id;
  GLCall(id = glCreateShader(type));
  const char* src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

  // Error handling
  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE) {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* message = (char*)alloca(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "Failed to compile "
              << (type == (unsigned int)GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
    std::cout << message << std::endl;
    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

unsigned int Shader::CreateShader(const std::string& vertex_shader,
                                  const std::string& fragment_shader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));
  return program;
}

int Shader::GetUniformLocation(const std::string& name) {
  if (uniform_location_cache_.find(name) != uniform_location_cache_.end()) {
    return uniform_location_cache_[name];
  }
  int location;
  GLCall(location = glGetUniformLocation(renderer_id_, name.c_str()));
  if (location == -1) {
    std::cout << "Warning: uniform '" << name << "' doesn't exits" << std::endl;
  }
  uniform_location_cache_[name] = location;
  return location;
}
