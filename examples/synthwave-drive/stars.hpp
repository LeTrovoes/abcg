#ifndef STARLAYERS_HPP_
#define STARLAYERS_HPP_

#include <random>
#include <vector>

#include "abcg.hpp"

class OpenGLWindow;

class Stars {
 public:
  void initialize(GLuint program);
  void update(float deltaTime);
  void paint(glm::mat4* m_viewMatrix, glm::mat4* m_projMatrix);
  void terminate();

 private:
  friend OpenGLWindow;

  GLuint program{};
  GLuint vao{};
  GLuint vbo{};

  GLint ul_opacity{};
  GLint ul_model_matrix{};
  GLint ul_view_matrix{};
  GLint ul_proj_matrix{};

  std::vector<glm::vec3> stars{}; // (x, y, opacity)
  std::default_random_engine random_engine;
  float pos_x_min = -30.0f;
  float pos_x_max = 30.0f;
  float pos_y_min = 0.0f;
  float pos_y_max = 10.0f;
};

#endif
