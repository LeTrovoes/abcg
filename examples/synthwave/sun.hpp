#ifndef SUN_HPP_
#define SUN_HPP_

#include "abcg.hpp"
#include "scene_element.hpp"

class OpenGLWindow;
class Sun : public SceneElement {
 public:
  void initialize(GLuint program) override;
  void paint() override;
  void terminate() override;

 private:
  friend OpenGLWindow;

  GLuint vao{};
  GLuint vbo{};

  GLint ul_model_matrix{};
  GLint ul_color{};

  int steps{100};
};

#endif
