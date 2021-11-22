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

  GLuint gradient_program = createProgramFromFile(
      getAssetsPath() + "gradient.vert", getAssetsPath() + "gradient.frag");
  gradient.initialize(gradient_program);

  GLuint stars_program = createProgramFromFile(getAssetsPath() + "stars.vert",
                                               getAssetsPath() + "stars.frag");
  stars.initialize(stars_program);

  ul_view_matrix = abcg::glGetUniformLocation(program, "viewMatrix");
  ul_proj_matrix = abcg::glGetUniformLocation(program, "projMatrix");
  ul_model_matrix = abcg::glGetUniformLocation(program, "modelMatrix");
  ul_color = abcg::glGetUniformLocation(program, "color");
  ul_shadow_range = abcg::glGetUniformLocation(program, "shadowRange");
  ul_shadow_offset = abcg::glGetUniformLocation(program, "shadowOffset");
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  abcg::glViewport(0, 0, viewport_width, viewport_height);
  abcg::glUseProgram(program);
  abcg::glFrontFace(GL_CCW);

  const auto aspect{static_cast<float>(viewport_width) /
                    static_cast<float>(viewport_height)};
  projMatrix = glm::perspective(glm::radians(50.0f), aspect, 0.1f, 20.0f);

  abcg::glUniformMatrix4fv(ul_proj_matrix, 1, GL_FALSE, &projMatrix[0][0]);
  abcg::glUniformMatrix4fv(ul_view_matrix, 1, GL_FALSE, &viewMatrix[0][0]);

  abcg::glUniform4f(ul_color, 1.0f, 1.0f, 1.0f, 1.0f);

  abcg::glUniform1f(ul_shadow_offset, -5);
  abcg::glUniform1f(ul_shadow_range, 1.3);

  abcg::glFrontFace(GL_CW);
  abcg::glUniformMatrix4fv(ul_model_matrix, 1, GL_FALSE,
                           &modelMatrixRight[0][0]);
  model.render();

  abcg::glFrontFace(GL_CCW);
  abcg::glUniformMatrix4fv(ul_model_matrix, 1, GL_FALSE,
                           &modelMatrixLeft[0][0]);
  model.render();

  abcg::glUniform1f(ul_shadow_offset, 0);
  abcg::glUniform1f(ul_shadow_range, 100);

  sun.paint();
  ground.paint();

  abcg::glUseProgram(0);

  stars.paint(&viewMatrix, &projMatrix);
  gradient.paint(&viewMatrix, &projMatrix);
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

  modelMatrixRight = glm::mat4{1};
  modelMatrixRight =
      glm::translate(modelMatrixRight, glm::vec3(dist, 2.0f, -2.0f));
  modelMatrixRight =
      glm::rotate(modelMatrixRight, static_cast<float>(2 * M_PI - rotation),
                  glm::vec3(0, 1, 0));
  modelMatrixRight = glm::scale(modelMatrixRight, glm::vec3(-1, 1, 1));

  modelMatrixLeft = glm::mat4{1};
  modelMatrixLeft =
      glm::translate(modelMatrixLeft, glm::vec3(-dist, 2.0f, -2.0f));
  modelMatrixLeft = glm::rotate(modelMatrixLeft, rotation, glm::vec3(0, 1, 0));

  viewMatrix = glm::lookAt(glm::vec3(0.0f, 2.15f, 5), glm::vec3(0.0f, 2.22f, 0),
                           glm::vec3(0.0f, 1.0f, 0.0f));
}
