#ifndef PYRAMID_HPP_
#define PYRAMID_HPP_

#include "abcg.hpp"

class Pyramid {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};
};

#endif
