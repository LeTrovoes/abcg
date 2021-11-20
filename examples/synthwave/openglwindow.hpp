#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include "abcg.hpp"
#include "model.hpp"
#include "ground.hpp"
#include "pyramid.hpp"
#include "rosa.hpp"
#include "stars.hpp"
#include "sun.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  friend Rosa;

  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  Model m_model;
  Ground m_ground;
  Rosa m_rosa;
  Pyramid m_pyramid;
  Stars stars;
  Sun sun;
  int m_trianglesToDraw{};

  float m_p1{};
  float m_p2{};

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_modelMatrix2{1.0f};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};

  float rotation{0};

  void update();
};

#endif
