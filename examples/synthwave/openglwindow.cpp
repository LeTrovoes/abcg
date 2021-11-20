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
  // abcg::glClearColor(0.055f, 0.145f, 0.306f, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "depth.vert",
                                    getAssetsPath() + "depth.frag");

  // Load model
  m_model.loadObj(getAssetsPath() + "bunny.obj");

  m_model.setupVAO(m_program);

  m_trianglesToDraw = m_model.getNumTriangles();

  m_pyramid.initializeGL(m_program);
  m_ground.initializeGL(m_program);

  GLuint rosa_program = createProgramFromFile(getAssetsPath() + "rosa.vert",
                                              getAssetsPath() + "rosa.frag");
  m_rosa.initializeGL(rosa_program);

  GLuint stars_program = createProgramFromFile(getAssetsPath() + "stars.vert",
                                               getAssetsPath() + "stars.frag");
  stars.initializeGL(stars_program);

  sun.initializeGL(m_program);
}

void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  abcg::glFrontFace(GL_CCW);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  abcg::glUseProgram(m_program);


        const auto aspect{static_cast<float>(m_viewportWidth) /
                          static_cast<float>(m_viewportHeight)};
        m_projMatrix =
            glm::perspective(glm::radians(50.0f), aspect, 0.1f, 20.0f);

  m_ground.paintGL();
  // m_pyramid.paintGL();

  // Get location of uniform variables (could be precomputed)
  const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  const GLint colorLoc{abcg::glGetUniformLocation(m_program, "color")};
  const GLint shadowRange{abcg::glGetUniformLocation(m_program, "shadowRange")};
  const GLint shadowOffset{abcg::glGetUniformLocation(m_program, "shadowOffset")};

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);

  abcg::glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);  // White

  abcg::glUniform1f(shadowOffset, -5);
  abcg::glUniform1f(shadowRange, 1.3);

  abcg::glFrontFace(GL_CW);
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);
  m_model.render(m_trianglesToDraw);

  abcg::glFrontFace(GL_CCW);
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix2[0][0]);
  m_model.render(m_trianglesToDraw);

  abcg::glUniform1f(shadowOffset, 0);
  abcg::glUniform1f(shadowRange, 100);
  sun.paintGL();

  abcg::glUseProgram(0);

  stars.paintGL(&m_viewMatrix, &m_projMatrix);
  m_rosa.paintGL(&m_viewMatrix, &m_projMatrix);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;
}

void OpenGLWindow::terminateGL() {
  m_model.terminateGL();
  m_ground.terminateGL();
  m_pyramid.terminateGL();
  m_rosa.terminateGL();
  stars.terminateGL();
  sun.terminateGL();
  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  m_ground.update(deltaTime);
  stars.update(deltaTime);

  rotation += deltaTime * 1;
  if (rotation >= 2 * M_PI) {
    rotation -= 2 * M_PI;
  }

  const auto aspect{static_cast<float>(m_viewportWidth) /
                    static_cast<float>(m_viewportHeight)};
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
