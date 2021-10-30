#ifndef BULLETS_HPP_
#define BULLETS_HPP_

#include <list>

#include "abcg.hpp"
#include "ship.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Bullets {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

  void update(Ship &ship, GameData &gamedata, float deltaTime, PlayerName);

 private:
  friend OpenGLWindow;

  GLuint gl_program{};
  GLuint gl_vao{};
  GLuint gl_vbo{};

  GLint ul_color{};
  GLint ul_rotation{};
  GLint ul_translation{};
  GLint ul_scale{};

  struct Bullet {
    bool dead{};
    glm::vec2 translation{glm::vec2(0)};
    glm::vec2 velocity{glm::vec2(0)};
  };

  float scale{0.009f};

  std::list<Bullet> bullets;
};

#endif
