#include "sun.hpp"

#include <cmath>
#include <cppitertools/itertools.hpp>

#include "core.h"
#include "range.hpp"

//  sun color: 1, 0.84, 0.24, 1

void Sun::initializeGL(GLuint program) {
  terminateGL();

  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");

  std::vector<glm::vec3> shape_vertices;
  shape_vertices.emplace_back(0, 0, 0);
  const auto step{M_PI * 2 / steps};
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    shape_vertices.emplace_back(5 * std::cos(angle), 5 * std::sin(angle), 0);
  }
  shape_vertices.push_back(shape_vertices.at(1));

  // Generate VBO
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, shape_vertices.size() * sizeof(glm::vec3),
                     shape_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // GLint positionAttribute{abcg::glGetAttribLocation(m_program,
  // "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  const GLint posAttrib{abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(posAttrib);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Sun::paintGL() {
  abcg::glBindVertexArray(m_vao);

  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3(0.0f, 5.05f, -14.8f));
  abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

  abcg::glUniform4f(m_colorLoc, 1, 0.84, 0.24, 1);

  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, steps + 2);

  abcg::glBindVertexArray(0);
}

void Sun::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

/* void Sun::update(float deltaTime) {

} */
