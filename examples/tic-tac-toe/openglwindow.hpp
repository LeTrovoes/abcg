#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <algorithm>
#include <array>

#include "tic-tac-toe.hpp"
#include "abcg.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {

 public:
  explicit OpenGLWindow(std::unique_ptr<TicTacToe> _game) {
    game = std::move(_game);
  };

 protected:
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  std::string getCellLabel(int index);

 private:
  std::unique_ptr<TicTacToe> game;
  std::array<float, 4> m_clearColor{0.906f, 0.910f, 0.918f, 1.0f};
};

#endif
