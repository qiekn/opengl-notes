#include "test_clear_color.h"
#include "imgui.h"
#include "renderer.h"

namespace test {
TestClearColor::TestClearColor() : clear_color_(0.2f, 0.3f, 0.3f, 1.0f) {}

TestClearColor::~TestClearColor() {}

void TestClearColor::OnUpdate(float dt) {}

void TestClearColor::OnRender() {
  GLCall(glClearColor(clear_color_[0], clear_color_[1], clear_color_[2], clear_color_[3]));
  GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void TestClearColor::OnImGuiRender() { ImGui::ColorEdit4("ClearColor", clear_color_); }
}  // namespace test
