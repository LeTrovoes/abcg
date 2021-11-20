#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcg.hpp"

class Ground {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();
  void update(float deltaTime);

 private:
  GLuint grid_VAO{};
  GLuint grid_VBO{};

  GLuint plane_VAO{};
  GLuint plane_VBO{};

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  float translation{0};
  float speed{1.0f};
  float translation_limit{1};
};

#endif
