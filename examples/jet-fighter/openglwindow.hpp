#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <array>

#include "abcg.hpp"
#include "bullets.hpp"
#include "imgui.h"
#include "ship.hpp"
#include "gamedata.hpp"
#include "health.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void paintGL() override;
  void paintUI() override;

 private:
  GLuint gl_objects_program{};
  GLuint healthProgram{};

  int viewport_width{600};
  int viewport_height{600};

  Bullets red_bullets;
  Ship red_ship;

  Bullets blu_bullets;
  Ship blu_ship;

  Health health;

  GameData gameData;

  ImFont* font{};

  void update();
  void checkColisions();
  void checkWinners();
  void restart();
};

#endif
