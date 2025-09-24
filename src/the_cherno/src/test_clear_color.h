#pragma once

#include "test.h"

namespace test {
class TestClearColor : public Test {
public:
  TestClearColor();
  virtual ~TestClearColor();

  void OnUpdate(float dt) override;
  void OnRender() override;
  void OnImGuiRender() override;

private:
  const char* label_;
  float clear_color_[4];
};
}  // namespace test
