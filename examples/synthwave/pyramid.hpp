#ifndef PYRAMID_HPP_
#define PYRAMID_HPP_

#include "abcg.hpp"
#include "scene_element.hpp"

class Pyramid : public SceneElement {
 public:
  void initialize(GLuint program) override;
  void paint() override;
  void terminate() override;

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};
};

#endif
