#ifndef SUN_HPP_
#define SUN_HPP_

#include <array>
#include <glm/fwd.hpp>
#include <vector>

#include "abcg.hpp"

class OpenGLWindow;

class Sun {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

  void update(float deltaTime);

 private:
  friend OpenGLWindow;

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};

  int steps{100};
};

#endif
