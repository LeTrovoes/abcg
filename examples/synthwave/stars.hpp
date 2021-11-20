#ifndef STARLAYERS_HPP_
#define STARLAYERS_HPP_

#include <array>
#include <glm/fwd.hpp>
#include <random>
#include <vector>

#include "abcg.hpp"

class OpenGLWindow;

class Stars {
 public:
  void initializeGL(GLuint program);
  void paintGL(glm::mat4* m_viewMatrix, glm::mat4* m_projMatrix);
  void terminateGL();

  void update(float deltaTime);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_opacityLoc{};
  GLint m_modelMatrixLoc{};
  GLint m_viewMatrixLoc{};
  GLint m_projMatrixLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};

  std::vector<glm::vec3> stars{};
  std::default_random_engine m_randomEngine;
};

#endif
