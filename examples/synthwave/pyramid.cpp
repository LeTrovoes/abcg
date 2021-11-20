#include "pyramid.hpp"

#include <cppitertools/itertools.hpp>

void Pyramid::initialize(GLuint program) {
  std::array vertices{
    glm::vec3(-2.22f, 0.0f, 0.0f),
    glm::vec3( 0.00f, 3.0f, 0.0f),
    glm::vec3( 0.00f, 0.0f, 0.0f)
  };

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO and bind vertex attributes
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  const GLint posAttrib{abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(posAttrib);
  abcg::glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);

  // Save location of uniform variables
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");
}

void Pyramid::paint() {
  // Draw a grid of tiles centered on the xz plane
  abcg::glBindVertexArray(m_VAO);

  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3(0, 0, -9.99));
  abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

  abcg::glUniform4f(m_colorLoc, 1.0f, 0, 0.5f, 1.0f);

  abcg::glDrawArrays(GL_TRIANGLES, 0, 3);

  abcg::glBindVertexArray(0);
}

void Pyramid::terminate() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}
