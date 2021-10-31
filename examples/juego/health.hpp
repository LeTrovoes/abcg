#ifndef HEALTH_HPP_
#define HEALTH_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Health {
 public:
  void initializeGL(GLuint program);
  void paintGL(int placement, PlayerName player);
  void terminateGL();

  void update();

 private:
  friend OpenGLWindow;

  GLuint gl_program{};
  GLuint gl_vao{};
  GLuint gl_vbo{};

  GLint ul_translation{};
  GLint ul_rotation{};
  GLint ul_scale{};
  GLint ul_color{};

  glm::vec4 color{0, 0.5, 1, 1};
  float rotation{};
  float scale{0.04f};
  glm::vec2 translation{glm::vec2(0)};
  glm::vec2 velocity{glm::vec2(0)};
};

#endif
