#include "ground.hpp"

#include <cppitertools/itertools.hpp>

void Ground::initialize(GLuint program) {
  terminate();

  initializeGrid(program);
  initializePlane(program);

  ul_model_matrix = abcg::glGetUniformLocation(program, "modelMatrix");
  ul_color = abcg::glGetUniformLocation(program, "color");
}

void Ground::paint() {
  const int N{50};

  abcg::glBindVertexArray(grid_vao);
  abcg::glUniform4f(ul_color, 0.375, 0.520f, 0.621f, 1.0f);

  // draw vertical lines
  for (const int i : iter::range(-N / 2, N / 2 + 1)) {
    float translation =
        static_cast<float>(i) * grid_spacing + grid_spacing / 2.0f;

    glm::mat4 model{1.0f};
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, translation));
    abcg::glUniformMatrix4fv(ul_model_matrix, 1, GL_FALSE, &model[0][0]);

    abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

  // draw horizontal lines
  for (const auto i : iter::range(-1, N + 1)) {
    float translation =
        static_cast<float>(-i) * grid_spacing + grid_translation;

    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, translation));
    abcg::glUniformMatrix4fv(ul_model_matrix, 1, GL_FALSE, &model[0][0]);

    abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

  abcg::glBindVertexArray(0);

  // draw plane
  abcg::glBindVertexArray(plane_vao);
  abcg::glUniform4f(ul_color, 0.113f, 0.070f, 0.196f, 1);

  glm::mat4 model{1.0f};
  abcg::glUniformMatrix4fv(ul_model_matrix, 1, GL_FALSE, &model[0][0]);

  abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  abcg::glBindVertexArray(0);
}

void Ground::update(float deltaTime) {
  grid_translation += speed * deltaTime;
  if (grid_translation >= grid_spacing) {
    grid_translation -= grid_spacing;
  }
}

void Ground::terminate() {
  abcg::glDeleteBuffers(1, &grid_vbo);
  abcg::glDeleteVertexArrays(1, &grid_vao);
}

void Ground::initializePlane(GLuint program) {
  std::array plane_vertices{
      glm::vec3(-50.0f, 0.0f, 50.0f), glm::vec3(50.0f, 0.0f, 50.0f),
      glm::vec3(-50.0f, 0.0f, -50.0f), glm::vec3(50.0f, 0.0f, -50.0f)};

  abcg::glGenBuffers(1, &plane_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices),
                     plane_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenVertexArrays(1, &plane_vao);
  abcg::glBindVertexArray(plane_vao);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);

  const GLint posAttrib{abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(posAttrib);
  abcg::glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);
}

void Ground::initializeGrid(GLuint program) {
  std::array line_vertices{
      glm::vec3(-30.0f, 0.01f, 0.03f), glm::vec3(30.0f, 0.01f, 0.03f),
      glm::vec3(-30.0f, 0.01f, -0.03f), glm::vec3(30.0f, 0.01f, -0.03f)};

  abcg::glGenBuffers(1, &grid_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices),
                     line_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenVertexArrays(1, &grid_vao);
  abcg::glBindVertexArray(grid_vao);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);

  const GLint posAttrib{abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(posAttrib);
  abcg::glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);
}
