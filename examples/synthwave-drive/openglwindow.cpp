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

  program_blinnphong = createProgramFromFile(
      getAssetsPath() + "blinnphong.vert", getAssetsPath() + "blinnphong.frag");

  program_blur = createProgramFromFile(getAssetsPath() + "blur.vert",
                                       getAssetsPath() + "blur.frag");

  model.loadObj(getAssetsPath() + "bunny.obj");

  model.setupVAO(program_blinnphong);
  sun.initialize(program);
  ground.initialize(program);

  GLuint gradient_program = createProgramFromFile(
      getAssetsPath() + "gradient.vert", getAssetsPath() + "gradient.frag");
  gradient.initialize(gradient_program);

  GLuint stars_program = createProgramFromFile(getAssetsPath() + "stars.vert",
                                               getAssetsPath() + "stars.frag");
  stars.initialize(stars_program);

  glGenFramebuffers(1, &glowFBO);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glowFBO);
  glGenTextures(2, &colorBuffers[0]);
  for (unsigned int i = 0; i < 2; i++) {
    glBindTexture(GL_TEXTURE_2D, colorBuffers.at(i));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, viewport_width, viewport_height,
                 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                           GL_TEXTURE_2D, colorBuffers.at(i), 0);
  }
  GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    fmt::print("Error creating FBO");
    exit(EXIT_FAILURE);
  }
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  std::array<glm::vec2, 4> positions{
      glm::vec2{+1, +1},
      glm::vec2{+1, -1},
      glm::vec2{-1, -1},
      glm::vec2{-1, +1},
  };

  abcg::glGenBuffers(1, &blur_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, blur_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &blur_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(blur_vao);

  abcg::glEnableVertexAttribArray(0);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, blur_vbo);
  abcg::glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
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

  /* GLOW */
  abcg::glUseProgram(program);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glowFBO);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);

  abcg::glClearColor(0, 0, 0, 0);
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  ground.paint();

  abcg::glUseProgram(program_blur);
  glBindVertexArray(blur_vao);

  const auto ul_blur_direction =
      abcg::glGetUniformLocation(program_blur, "blurDirection");

  // FIRST BLUE 0 -> 1
  glDrawBuffer(GL_COLOR_ATTACHMENT1);
  glBindTexture(GL_TEXTURE_2D, colorBuffers.at(0));
  abcg::glUniform2f(ul_blur_direction, 1, 0);
  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  // SECOND BLUR 1 -> 0
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  glBindTexture(GL_TEXTURE_2D, colorBuffers.at(1));
  abcg::glUniform2f(ul_blur_direction, 0, 1);
  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  // THIRD BLUR 0 -> 1
  glDrawBuffer(GL_COLOR_ATTACHMENT1);
  glBindTexture(GL_TEXTURE_2D, colorBuffers.at(0));
  abcg::glUniform2f(ul_blur_direction, 1, 0);
  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  //   /* SECOND BLUR 1 -> 0 */
  // glDrawBuffer(GL_COLOR_ATTACHMENT0);
  // glBindTexture(GL_TEXTURE_2D, colorBuffers.at(1));
  // abcg::glUniform2f(ul_blur_direction, 0, 1);
  // abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  // /* THIRD BLUR 0 -> 1 */
  // glDrawBuffer(GL_COLOR_ATTACHMENT1);
  // glBindTexture(GL_TEXTURE_2D, colorBuffers.at(0));
  // abcg::glUniform2f(ul_blur_direction, 1, 0);
  // abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  //   /* SECOND BLUR 1 -> 0 */
  // glDrawBuffer(GL_COLOR_ATTACHMENT0);
  // glBindTexture(GL_TEXTURE_2D, colorBuffers.at(1));
  // abcg::glUniform2f(ul_blur_direction, 0, 1);
  // abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  // /* THIRD BLUR 0 -> 1 */
  // glDrawBuffer(GL_COLOR_ATTACHMENT1);
  // glBindTexture(GL_TEXTURE_2D, colorBuffers.at(0));
  // abcg::glUniform2f(ul_blur_direction, 1, 0);
  // abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  // LAST BUFFER 1 -> SCREEN //
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK_LEFT);
  abcg::glUniform2f(ul_blur_direction, 0, 1);
  //abcg::glClearColor(0.055f, 0.145f, 0.306f, 1.0f);
   abcg::glClearColor(0.113f, 0.070f, 0.196f, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D, colorBuffers.at(1));
  glEnable(GL_BLEND);
  abcg::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glBindVertexArray(0);
  glDisable(GL_BLEND);

  /* END GLOW */
  abcg::glUseProgram(program);

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

  abcg::glUniform1f(ul_shadow_offset, 0);
  abcg::glUniform1f(ul_shadow_range, 100);

  sun.paint();
  // ground.paint();

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

  auto lightDirRotated = glm::vec4(0, -0.775f, 1.548f, 0);
  abcg::glUniform4fv(ul_light_dir, 1, &lightDirRotated.x);
  abcg::glUniform4fv(ul_Ia, 1, &m_Ia.x);
  abcg::glUniform4fv(ul_Id, 1, &m_Id.x);
  abcg::glUniform4fv(ul_Is, 1, &m_Is.x);

  // Set uniform variables of the current object
  abcg::glUniformMatrix4fv(ul_model_matrix, 1, GL_FALSE, &modelMatrixCar[0][0]);

  auto modelViewMatrix{glm::mat3(viewMatrix * modelMatrixCar)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(ul_normal_matrix, 1, GL_FALSE, &normalMatrix[0][0]);

  abcg::glUniform1f(ul_shininess, m_shininess);
  abcg::glUniform4fv(ul_Ka, 1, &m_Ka.x);
  abcg::glUniform4fv(ul_Kd, 1, &m_Kd.x);
  abcg::glUniform4fv(ul_Ks, 1, &m_Ks.x);

  car.render();

  abcg::glUseProgram(program_blinnphong);

  ul_view_matrix = abcg::glGetUniformLocation(program_blinnphong, "viewMatrix");
  ul_proj_matrix = abcg::glGetUniformLocation(program_blinnphong, "projMatrix");
  ul_model_matrix =
      abcg::glGetUniformLocation(program_blinnphong, "modelMatrix");
  ul_normal_matrix =
      abcg::glGetUniformLocation(program_blinnphong, "normalMatrix");
  ul_light_dir =
      abcg::glGetUniformLocation(program_blinnphong, "lightDirWorldSpace");
  ul_shininess = abcg::glGetUniformLocation(program_blinnphong, "shininess");
  ul_Ia = abcg::glGetUniformLocation(program_blinnphong, "Ia");
  ul_Id = abcg::glGetUniformLocation(program_blinnphong, "Id");
  ul_Is = abcg::glGetUniformLocation(program_blinnphong, "Is");
  ul_Ka = abcg::glGetUniformLocation(program_blinnphong, "Ka");
  ul_Kd = abcg::glGetUniformLocation(program_blinnphong, "Kd");
  ul_Ks = abcg::glGetUniformLocation(program_blinnphong, "Ks");

  const auto kA = glm::vec4(0, 0, 0, 0);
  const auto kD = glm::vec4(1, 1, 1, 0);
  const auto kS = glm::vec4(1, 1, 1, 0);
  abcg::glUniform4fv(ul_Ka, 1, &kA.x);
  abcg::glUniform4fv(ul_Kd, 1, &kD.x);
  abcg::glUniform4fv(ul_Ks, 1, &kS.x);

  abcg::glUniform4fv(ul_Ia, 1, &kA.x);
  abcg::glUniform4fv(ul_Id, 1, &kD.x);
  abcg::glUniform4fv(ul_Is, 1, &kS.x);

  abcg::glUniform1f(ul_shininess, m_shininess);

  // lightDirRotated = glm::vec4(0.23991199, -0.6852303, -1.5725459, 0);
  lightDirRotated = glm::vec4(-1, -1, -1, 0);
  abcg::glUniform4fv(ul_light_dir, 1, &lightDirRotated.x);

  abcg::glUniformMatrix4fv(ul_proj_matrix, 1, GL_FALSE, &projMatrix[0][0]);
  abcg::glUniformMatrix4fv(ul_view_matrix, 1, GL_FALSE, &viewMatrix[0][0]);

  abcg::glFrontFace(GL_CW);
  abcg::glUniformMatrix4fv(ul_model_matrix, 1, GL_FALSE,
                           &modelMatrixRight[0][0]);
  modelViewMatrix = glm::mat3(viewMatrix * modelMatrixRight);
  normalMatrix = glm::inverseTranspose(modelViewMatrix);
  abcg::glUniformMatrix3fv(ul_normal_matrix, 1, GL_FALSE, &normalMatrix[0][0]);
  model.render();

  abcg::glFrontFace(GL_CCW);
  abcg::glUniformMatrix4fv(ul_model_matrix, 1, GL_FALSE,
                           &modelMatrixLeft[0][0]);
  modelViewMatrix = glm::mat3(viewMatrix * modelMatrixLeft);
  normalMatrix = glm::inverseTranspose(modelViewMatrix);
  abcg::glUniformMatrix3fv(ul_normal_matrix, 1, GL_FALSE, &normalMatrix[0][0]);
  model.render();

  abcg::glUseProgram(0);
}

void OpenGLWindow::resizeGL(int width, int height) {
  viewport_width = width;
  viewport_height = height;
  for (unsigned int i = 0; i < 2; i++) {
    glBindTexture(GL_TEXTURE_2D, colorBuffers.at(i));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, viewport_width, viewport_height,
                 0, GL_RGBA, GL_FLOAT, nullptr);
  }
  glBindTexture(GL_TEXTURE_2D, 0);
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
  float dist = fmax(aspect * 1.5f, 1.0f);

  modelMatrixRight = glm::mat4{1};
  modelMatrixRight =
      glm::translate(modelMatrixRight, glm::vec3(dist, 1.0f, -2.0f));
  modelMatrixRight =
      glm::rotate(modelMatrixRight, static_cast<float>(2 * M_PI - rotation),
                  glm::vec3(0, 1, 0));
  modelMatrixRight = glm::scale(modelMatrixRight, glm::vec3(-1, 1, 1));

  modelMatrixLeft = glm::mat4{1};
  modelMatrixLeft =
      glm::translate(modelMatrixLeft, glm::vec3(-dist, 1.0f, -2.0f));
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
  modelMatrixCar =
      glm::translate(modelMatrixCar, glm::vec3(car_position, 0.25f, 0.0f));
  modelMatrixCar =
      glm::rotate(modelMatrixCar, static_cast<float>(M_PI), glm::vec3(0, 1, 0));
}
