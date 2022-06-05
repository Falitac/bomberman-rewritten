#include <fmt/core.h>
#include <fmt/color.h>
#include "App.hpp"
#include "utility/GLUtils.hpp"

int main(int argc, char** argv) {
  try {
    App app;
    app.run();
  } catch(const std::exception& e) {
    fmt::print(stderr, fmt::fg(fmt::color::red), "{}", e.what());
    return EXIT_FAILURE;
  }

  fmt::print(fmt::fg(fmt::color::green), "Done & Dust\n");

  return EXIT_SUCCESS;
}