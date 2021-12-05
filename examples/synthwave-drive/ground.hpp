#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcg.hpp"
#include "scene_element.hpp"

class Ground : public SceneElement {
 public:
  void initialize(GLuint program) override;
  void update(float deltaTime) override;
  void paint() override;
  void terminate() override;

 private:
  void initializePlane(GLuint program);
  GLuint plane_vao{};
  GLuint plane_vbo{};

  void initializeGrid(GLuint program);
  GLuint grid_vao{};
  GLuint grid_vbo{};

  GLint ul_model_matrix{};
  GLint ul_color{};

  float grid_translation{0};
  float speed{3};
  const float grid_spacing{1};
};

#endif
