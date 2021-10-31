#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;
class Bullets;

class Ship {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

  void hit();
  void update(PlayerInput &input, float deltaTime);
  void restart();

 private:
  friend OpenGLWindow;
  friend Bullets;

  GLuint gl_program{};
  GLuint gl_vao{};
  GLuint gl_vbo{};

  GLint ul_translation{};
  GLint ul_rotation{};
  GLint ul_scale{};
  GLint ul_color{};

  glm::vec4 color{0,0.5,1,1};
  float rotation{};
  float scale{0.04f};

  PlayerName player_name;
  int life{0};

  glm::vec2 translation{glm::vec2(0)};
  glm::vec2 velocity{glm::vec2(0)};

  bool is_immune{false};
  abcg::ElapsedTimer immunity_timer;
  abcg::ElapsedTimer bullet_cooldown_timer;
};
#endif