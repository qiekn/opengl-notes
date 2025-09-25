#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace test {
class Test {
public:
  Test() {}
  virtual ~Test() {}

  virtual void OnUpdate(float dt) {}
  virtual void OnRender() {}
  virtual void OnImGuiRender() {}
};

class TestMenu : public Test {
public:
  TestMenu(Test*& current_test);

  void OnImGuiRender() override;

  template <typename T>
  void RegisterTest(const std::string& name) {
    std::cout << "Register test: " << name << std::endl;

    tests_.push_back(std::make_pair(name, []() { return new T(); }));
  }

private:
  Test*& current_test_;
  std::vector<std::pair<std::string, std::function<Test*()>>> tests_;
};
}  // namespace test
