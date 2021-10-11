#include <fmt/core.h>

#include "abcg.hpp"
#include "openglwindow.hpp"

int main(int argc, char **argv) {


  try {
    // Create application instance
    abcg::Application app(argc, argv);

    // Create game instance
    auto game{std::make_unique<TicTacToe>()};

    // Create OpenGL window
    auto window = std::make_unique<OpenGLWindow>(std::move(game));
    window->setWindowSettings(
        {.showFullscreenButton = false, .title = "Tic-Tac-Toe"});
    window->setOpenGLSettings({.vsync = true});

    // Run application
    app.run(std::move(window));
  } catch (const abcg::Exception &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
}
