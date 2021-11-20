#include "rosa.hpp"

#include <cppitertools/itertools.hpp>
#include <cstdio>

#include "core.h"

void Rosa::initializeGL(GLuint _program) {
  m_program = _program;

  // Unit quad on the xz plane
  std::array vertices{glm::vec2(50.0f, 0.0f), glm::vec2(50.0f, 12.0f),
                      glm::vec2(-50.0f, 12.0f), glm::vec2(-50.0, -0.0f)};
 // 0.97
  std::array colors{glm::vec4(0.976f, 0.161f, 0.578f, 0.9f),
                    glm::vec4(0.976f, 0.161f, 0.578f, -0.15f),
                    glm::vec4(0.976f, 0.161f, 0.578f, -0.15f),
                    glm::vec4(0.976f, 0.161f, 0.578f, 0.9f)};

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate VBO
  abcg::glGenBuffers(1, &oto_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, oto_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4),
                     colors.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO and bind vertex attributes
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  const GLint posAttrib{abcg::glGetAttribLocation(m_program, "inPosition")};
  abcg::glEnableVertexAttribArray(posAttrib);
  abcg::glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  const GLint colorAttrib{abcg::glGetAttribLocation(m_program, "inColor")};
  abcg::glEnableVertexAttribArray(colorAttrib);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, oto_VBO);
  abcg::glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindVertexArray(0);
}

void Rosa::paintGL(glm::mat4* m_viewMatrix, glm::mat4* m_projMatrix) {
  abcg::glUseProgram(m_program);
  abcg::glBindVertexArray(m_VAO);

    const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};

  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &(*m_viewMatrix)[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &(*m_projMatrix)[0][0]);

  glm::mat4 model = glm::mat4{1};
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -14.7f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glDisable(GL_BLEND);
  abcg::glUseProgram(0);
}

void Rosa::terminateGL() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}
