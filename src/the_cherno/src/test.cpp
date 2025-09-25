#include "test.h"
#include "imgui.h"

namespace test {
TestMenu::TestMenu(Test*& current_test) : current_test_(current_test) {}

void TestMenu::OnImGuiRender() {
  for (auto& test : tests_) {
    if (ImGui::Button(test.first.c_str())) {
      current_test_ = test.second();
    }
  }
}
}  // namespace test
