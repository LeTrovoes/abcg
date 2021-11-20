#include "ground.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>

void Ground::initializeGL(GLuint program) {
  // Unit quad on the xz plane
  std::array vertices{
      glm::vec3(-30.0f, 0.01f, 0.03f), glm::vec3(30.0f, 0.01f, 0.03f),
      glm::vec3(-30.0f, 0.01f, -0.03f), glm::vec3(30.0f, 0.01f, -0.03f)};

  // Generate VBO
  abcg::glGenBuffers(1, &grid_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, grid_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO and bind vertex attributes
  abcg::glGenVertexArrays(1, &grid_VAO);
  abcg::glBindVertexArray(grid_VAO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, grid_VBO);
  const GLint posAttrib{abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(posAttrib);
  abcg::glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);


  std::array plane_vertices{
      glm::vec3(-50.0f, 0.0f, 50.0f), glm::vec3(50.0f, 0.0f, 50.0f),
      glm::vec3(-50.0f, 0.0f, -50.0f), glm::vec3(50.0f, 0.0f, -50.0f)};

  // Generate VBO
  abcg::glGenBuffers(1, &plane_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, plane_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO and bind vertex attributes
  abcg::glGenVertexArrays(1, &plane_VAO);
  abcg::glBindVertexArray(plane_VAO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, plane_VBO);
  abcg::glEnableVertexAttribArray(posAttrib);
  abcg::glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);

  // Save location of uniform variables
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");
}

void Ground::paintGL() {
  // Draw a grid of tiles centered on the xz plane
  const int N{50};

  abcg::glBindVertexArray(grid_VAO);
  abcg::glUniform4f(m_colorLoc, 0.375, 0.520f, 0.621f, 1.0f);
  // columns
  for (const auto i : iter::range(-N/2, N/2 + 1)) {
    // Set model matrix
    glm::mat4 model{1.0f};
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, i + 0.5f));
    abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

    // Set color (checkerboard pattern)



    abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

  // lines
  for (const auto i : iter::range(-1, N + 1)) {
    // Set model matrix
    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -i + translation));
    abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

    abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
  abcg::glBindVertexArray(0);


  abcg::glBindVertexArray(plane_VAO);
  abcg::glUniform4f(m_colorLoc, 0.113f, 0.070f, 0.196f, 1);
  glm::mat4 model{1.0f};
  abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  abcg::glBindVertexArray(0);
}

void Ground::update(float deltaTime) {
  translation += speed * deltaTime;
  if (translation >= translation_limit) {
    translation = 0;
  }
}

void Ground::terminateGL() {
  abcg::glDeleteBuffers(1, &grid_VBO);
  abcg::glDeleteVertexArrays(1, &grid_VAO);
}
