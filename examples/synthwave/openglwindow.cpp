#include "openglwindow.hpp"

#include <imgui.h>

#include <algorithm>
#include <cmath>
#include <cppitertools/itertools.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>

#include "abcg_openglfunctions.hpp"
#include "core.h"

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0.055f, 0.145f, 0.306f, 1.0f);
  abcg::glEnable(GL_DEPTH_TEST);

  program = createProgramFromFile(getAssetsPath() + "depth.vert",
                                    getAssetsPath() + "depth.frag");

  model.loadObj(getAssetsPath() + "bunny.obj");

  model.setupVAO(program);
  sun.initialize(program);
  ground.initialize(program);
  pyramid.initialize(program);

  GLuint gradient_program = createProgramFromFile(getAssetsPath() + "rosa.vert",
                                              getAssetsPath() + "rosa.frag");
  gradient.initialize(gradient_program);

  GLuint stars_program = createProgramFromFile(getAssetsPath() + "stars.vert",
                                               getAssetsPath() + "stars.frag");
  stars.initialize(stars_program);
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  abcg::glViewport(0, 0, viewport_width, viewport_height);
  abcg::glUseProgram(program);
  abcg::glFrontFace(GL_CCW);

  const auto aspect{static_cast<float>(viewport_width) /
                    static_cast<float>(viewport_height)};
  m_projMatrix =
      glm::perspective(glm::radians(50.0f), aspect, 0.1f, 20.0f);

  // Get location of uniform variables (could be precomputed)
  const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(program, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(program, "modelMatrix")};
  const GLint colorLoc{abcg::glGetUniformLocation(program, "color")};
  const GLint shadowRange{abcg::glGetUniformLocation(program, "shadowRange")};
  const GLint shadowOffset{abcg::glGetUniformLocation(program, "shadowOffset")};

  // set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);

  abcg::glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

  abcg::glUniform1f(shadowOffset, -5);
  abcg::glUniform1f(shadowRange, 1.3);

  abcg::glFrontFace(GL_CW);
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);
  model.render();

  abcg::glFrontFace(GL_CCW);
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix2[0][0]);
  model.render();

  abcg::glUniform1f(shadowOffset, 0);
  abcg::glUniform1f(shadowRange, 100);

  sun.paint();
  ground.paint();

  abcg::glUseProgram(0);

  stars.paint(&m_viewMatrix, &m_projMatrix);
  gradient.paint(&m_viewMatrix, &m_projMatrix);
}

void OpenGLWindow::resizeGL(int width, int height) {
  viewport_width = width;
  viewport_height = height;
}

void OpenGLWindow::terminateGL() {
  sun.terminate();
  stars.terminate();
  model.terminateGL();
  ground.terminate();
  pyramid.terminate();
  gradient.terminate();
  abcg::glDeleteProgram(program);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  ground.update(deltaTime);
  stars.update(deltaTime);

  rotation += deltaTime * 1;
  if (rotation >= 2 * M_PI) {
    rotation -= 2 * M_PI;
  }

  const auto aspect{static_cast<float>(viewport_width) /
                    static_cast<float>(viewport_height)};
  float dist = fmax(aspect * 1.90f, 2.0f);

  // direita (->)
  m_modelMatrix = glm::mat4{1};
  m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(dist, 2.0f, -2.0f));
  m_modelMatrix = glm::rotate(m_modelMatrix, static_cast<float>(2*M_PI - rotation), glm::vec3(0, 1, 0));
  m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(-1, 1, 1));

  // esquerda (<-)
  m_modelMatrix2 = glm::mat4{1};
  m_modelMatrix2 = glm::translate(m_modelMatrix2, glm::vec3(-dist, 2.0f, -2.0f));
  m_modelMatrix2 = glm::rotate(m_modelMatrix2, rotation, glm::vec3(0, 1, 0));
  // m_modelMatrix2 = glm::scale(m_modelMatrix2, glm::vec3(-1, 1, -1));

  m_viewMatrix =
      glm::lookAt(glm::vec3(0.0f, 2.15f, 5), glm::vec3(0.0f, 2.22f, 0),
                  glm::vec3(0.0f, 1.0f, 0.0f));
}
