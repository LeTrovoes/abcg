#include "sun.hpp"

#include <cppitertools/itertools.hpp>

void Sun::initialize(GLuint program) {
  terminate();

  ul_model_matrix = abcg::glGetUniformLocation(program, "modelMatrix");
  ul_color = abcg::glGetUniformLocation(program, "color");

  // create circle
  std::vector<glm::vec3> sun_vertices;
  sun_vertices.emplace_back(0, 0, 0);
  const auto step{M_PI * 2 / steps};
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    sun_vertices.emplace_back(5 * std::cos(angle), 5 * std::sin(angle), 0);
  }
  sun_vertices.push_back(sun_vertices.at(1));

  abcg::glGenBuffers(1, &vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     static_cast<long>(sun_vertices.size() * sizeof(glm::vec3)),
                     sun_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenVertexArrays(1, &vao);
  abcg::glBindVertexArray(vao);

  const GLint posAttrib{abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(posAttrib);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, vbo);
  abcg::glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindVertexArray(0);
}

void Sun::paint() {
  abcg::glBindVertexArray(vao);

  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3(0.0f, 5.05f, -14.8f));
  abcg::glUniformMatrix4fv(ul_model_matrix, 1, GL_FALSE, &model[0][0]);

  abcg::glUniform4f(ul_color, 1, 0.84, 0.24, 1);

  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, steps + 2);

  abcg::glBindVertexArray(0);
}

void Sun::terminate() {
  abcg::glDeleteBuffers(1, &vbo);
  abcg::glDeleteVertexArrays(1, &vao);
}
