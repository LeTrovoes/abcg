#ifndef GRADIENT_HPP_
#define GRADIENT_HPP_

#include "abcg.hpp"

class Gradient {
 public:
  void initialize(GLuint _program);
  void paint(glm::mat4* m_viewMatrix, glm::mat4* m_projMatrix) const;
  void terminate();

 private:
  GLuint program{};
  GLuint vao{};
  GLuint vbo_position{};
  GLuint vbo_color{};

  GLint ul_model_matrix{};
  GLint ul_view_matrix{};
  GLint ul_proj_matrix{};
};

#endif
