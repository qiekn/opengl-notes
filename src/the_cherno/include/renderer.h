#pragma once

#include <alloca.h>
#include <iostream>  // IWYU pragma: keep
#include "glad/gl.h"
#include "index_buffer.h"
#include "shader.h"
#include "vertex_array.h"

#if defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK() __builtin_trap()
#endif

#define ASSERT(x)                              \
  if (!(x)) {                                  \
    std::cout << "Assert Failed" << std::endl; \
    DEBUG_BREAK();                             \
  }

#define GLCall(x)                              \
  do {                                         \
    GLClearError();                            \
    x;                                         \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)); \
  } while (0);

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
  void Clear() const;
  void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
