#pragma once

namespace test {
class Test {
public:
  Test() {}
  virtual ~Test() {}

  virtual void OnUpdate(float dt) {}
  virtual void OnRender() {}
  virtual void OnImGuiRender() {}
};
}  // namespace test
