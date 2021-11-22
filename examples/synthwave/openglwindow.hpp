#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include "abcg.hpp"
#include "model.hpp"
#include "ground.hpp"
#include "gradient.hpp"
#include "stars.hpp"
#include "sun.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint program{};

  int viewport_width{};
  int viewport_height{};

  Sun sun;
  Stars stars;
  Model model;
  Ground ground;
  Gradient gradient;

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_modelMatrix2{1.0f};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};

  float rotation{0};

  void update();
};

#endif
