#include "stars.hpp"

#include <cmath>
#include <cppitertools/itertools.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "core.h"
#include "range.hpp"

void Stars::initializeGL(GLuint program) {
  terminateGL();

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;
  m_opacityLoc = abcg::glGetUniformLocation(m_program, "opacity");
  m_modelMatrixLoc = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_viewMatrixLoc = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLoc = abcg::glGetUniformLocation(m_program, "projMatrix");

  auto &re{m_randomEngine};
  std::uniform_real_distribution<float> distPos(-20.0f, 20.0f);
  std::uniform_real_distribution<float> distPosY(0.0f, 10.0f);
  std::uniform_real_distribution<float> distIntensity(0.0f, 1.0);

  for ([[maybe_unused]] const auto i : iter::range(0, 100)) {
    stars.emplace_back(distPos(re), distPosY(re), distIntensity(re));
  }

  std::vector<glm::vec2> shape_vertices(0);
  shape_vertices.emplace_back(0, 0);
  const auto step{M_PI * 2 / 8};
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    shape_vertices.emplace_back(0.05 * std::cos(angle),
                                0.05 * std::sin(angle));
  }
  shape_vertices.push_back(shape_vertices.at(1));

  /*   for (const auto i : iter::range(0, 30)) {
    }
    std::vector<glm::vec3> data(0);
   */
  // Generate VBO
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, shape_vertices.size() * sizeof(glm::vec2),
                     shape_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // GLint positionAttribute{abcg::glGetAttribLocation(m_program,
  // "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(0);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Stars::paintGL(glm::mat4 *m_viewMatrix, glm::mat4 *m_projMatrix) {
  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (const auto &star : stars) {
    glm::mat4 model = glm::mat4{1};
    model = glm::translate(model, glm::vec3(star.x, star.y, -14.9f));

    abcg::glUniform1f(m_opacityLoc, star.z * 2.0f * M_PI);
    abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
    abcg::glUniformMatrix4fv(m_viewMatrixLoc, 1, GL_FALSE,
                             &(*m_viewMatrix)[0][0]);
    abcg::glUniformMatrix4fv(m_projMatrixLoc, 1, GL_FALSE,
                             &(*m_projMatrix)[0][0]);

    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 8 + 2);
  }

  glDisable(GL_BLEND);
  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Stars::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Stars::update(float deltaTime) {
  auto &re{m_randomEngine};
  std::uniform_real_distribution<float> distPos(-30.0f, 30.0f);
  std::uniform_real_distribution<float> distPosY(0.0f, 10.0f);
  for (auto &star : stars) {
    star.z += 0.1f * deltaTime;
    if (star.z >= 1.0) {
      star.z = 0;
      star.x = distPos(re);
      star.y = distPosY(re);
    }
  }
}
