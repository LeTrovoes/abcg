#include "openglwindow.hpp"

#include <imgui.h>

#include <algorithm>
#include <cmath>
#include <cppitertools/itertools.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/trigonometric.hpp>

#include "abcg_openglfunctions.hpp"
#include "core.h"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      input.set(static_cast<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      input.reset(static_cast<size_t>(Input::Right));
  }
}

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0.055f, 0.145f, 0.306f, 1.0f);
  abcg::glEnable(GL_DEPTH_TEST);

  program_texture = createProgramFromFile(getAssetsPath() + "texture.vert",
                                          getAssetsPath() + "texture.frag");

  loadModel(getAssetsPath() + "delorean.obj");

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
}

void OpenGLWindow::loadModel(std::string_view path) {
  car.terminateGL();

  car.loadDiffuseTexture(getAssetsPath() + "delorean.png");
  car.loadObj(path);
  car.setupVAO(program_texture);

  // Use material properties from the loaded model
  m_Ka = car.getKa();
  m_Kd = car.getKd();
  m_Ks = car.getKs();
  m_shininess = car.getShininess();
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  abcg::glViewport(0, 0, viewport_width, viewport_height);
  abcg::glUseProgram(program);
  abcg::glFrontFace(GL_CCW);

  ul_view_matrix = abcg::glGetUniformLocation(program, "viewMatrix");
  ul_proj_matrix = abcg::glGetUniformLocation(program, "projMatrix");
  ul_model_matrix = abcg::glGetUniformLocation(program, "modelMatrix");
  ul_color = abcg::glGetUniformLocation(program, "color");
  ul_shadow_range = abcg::glGetUniformLocation(program, "shadowRange");
  ul_shadow_offset = abcg::glGetUniformLocation(program, "shadowOffset");

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

  abcg::glUseProgram(program_texture);

  // Get location of uniform variables
  ul_view_matrix = abcg::glGetUniformLocation(program_texture, "viewMatrix");
  ul_proj_matrix = abcg::glGetUniformLocation(program_texture, "projMatrix");
  ul_model_matrix = abcg::glGetUniformLocation(program_texture, "modelMatrix");
  ul_normal_matrix =
      abcg::glGetUniformLocation(program_texture, "normalMatrix");
  ul_light_dir =
      abcg::glGetUniformLocation(program_texture, "lightDirWorldSpace");
  ul_shininess = abcg::glGetUniformLocation(program_texture, "shininess");
  ul_Ia = abcg::glGetUniformLocation(program_texture, "Ia");
  ul_Id = abcg::glGetUniformLocation(program_texture, "Id");
  ul_Is = abcg::glGetUniformLocation(program_texture, "Is");
  ul_Ka = abcg::glGetUniformLocation(program_texture, "Ka");
  ul_Kd = abcg::glGetUniformLocation(program_texture, "Kd");
  ul_Ks = abcg::glGetUniformLocation(program_texture, "Ks");
  ul_diffuseTex = abcg::glGetUniformLocation(program_texture, "diffuseTex");
  ul_mappingMode = abcg::glGetUniformLocation(program_texture, "mappingMode");

  abcg::glUniformMatrix4fv(ul_view_matrix, 1, GL_FALSE, &viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(ul_proj_matrix, 1, GL_FALSE, &projMatrix[0][0]);
  abcg::glUniform1i(ul_diffuseTex, 0);
  abcg::glUniform1i(ul_mappingMode, 3);

  const auto lightDirRotated =
      glm::vec4(0, -0.775f, 1.548f, 0);
  abcg::glUniform4fv(ul_light_dir, 1, &lightDirRotated.x);
  abcg::glUniform4fv(ul_Ia, 1, &m_Ia.x);
  abcg::glUniform4fv(ul_Id, 1, &m_Id.x);
  abcg::glUniform4fv(ul_Is, 1, &m_Is.x);

  // Set uniform variables of the current object
  abcg::glUniformMatrix4fv(ul_model_matrix, 1, GL_FALSE, &modelMatrixCar[0][0]);

  const auto modelViewMatrix{glm::mat3(viewMatrix * modelMatrixCar)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(ul_normal_matrix, 1, GL_FALSE, &normalMatrix[0][0]);

  abcg::glUniform1f(ul_shininess, m_shininess);
  abcg::glUniform4fv(ul_Ka, 1, &m_Ka.x);
  abcg::glUniform4fv(ul_Kd, 1, &m_Kd.x);
  abcg::glUniform4fv(ul_Ks, 1, &m_Ks.x);

  car.render();

  abcg::glUseProgram(0);
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

  viewMatrix =
      glm::lookAt(glm::vec3(0.0f, 0.7f, 2.5f), glm::vec3(0.0f, 0.9f, 0),
                  glm::vec3(0.0f, 1.0f, 0.0f));


  if (input[static_cast<size_t>(Input::Left)]) {
    car_position -= deltaTime * 1;
  }

  if (input[static_cast<size_t>(Input::Right)]) {
    car_position += deltaTime * 1;
  }

  if (car_position > +1.95f) car_position = +1.95f;
  if (car_position < -1.95f) car_position = -1.95f;

  modelMatrixCar = glm::mat4{1};
  // glm::scale(modelMatrixCar, glm::vec3(2));
  modelMatrixCar = glm::translate(modelMatrixCar, glm::vec3(car_position, 0.25f, 0.0f));
  modelMatrixCar =
      glm::rotate(modelMatrixCar, static_cast<float>(M_PI), glm::vec3(0, 1, 0));
}
