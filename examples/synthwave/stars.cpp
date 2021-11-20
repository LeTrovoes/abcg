#include "stars.hpp"

#include <cmath>
#include <cppitertools/itertools.hpp>
#include <glm/ext/matrix_transform.hpp>

void Stars::initialize(GLuint _program) {
  terminate();

  random_engine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  program = _program;
  ul_opacity = abcg::glGetUniformLocation(program, "opacity");
  ul_model_matrix = abcg::glGetUniformLocation(program, "modelMatrix");
  ul_view_matrix = abcg::glGetUniformLocation(program, "viewMatrix");
  ul_proj_matrix = abcg::glGetUniformLocation(program, "projMatrix");

  auto &re{random_engine};
  std::uniform_real_distribution<float> dist_pos_x(pos_x_min, pos_x_max);
  std::uniform_real_distribution<float> dist_pos_y(pos_y_min, pos_y_max);
  std::uniform_real_distribution<float> dist_opacity(0.0f, 1.0);

  for ([[maybe_unused]] const auto i : iter::range(0, 100)) {
    stars.emplace_back(dist_pos_x(re), dist_pos_y(re), dist_opacity(re));
  }

  // create circle
  std::vector<glm::vec2> shape_vertices(0);
  shape_vertices.emplace_back(0, 0);
  const auto step{M_PI * 2 / 8};
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    shape_vertices.emplace_back(0.05 * std::cos(angle), 0.05 * std::sin(angle));
  }
  shape_vertices.push_back(shape_vertices.at(1));

  abcg::glGenBuffers(1, &vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, vbo);
  abcg::glBufferData(
      GL_ARRAY_BUFFER,
      static_cast<long>(shape_vertices.size() * sizeof(glm::vec2)),
      shape_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenVertexArrays(1, &vao);
  abcg::glBindVertexArray(vao);

  abcg::glEnableVertexAttribArray(0);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, vbo);
  abcg::glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindVertexArray(0);
}

void Stars::paint(glm::mat4 *m_viewMatrix, glm::mat4 *m_projMatrix) {
  abcg::glUseProgram(program);
  abcg::glBindVertexArray(vao);

  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (const auto &star : stars) {
    glm::mat4 model = glm::mat4{1};
    model = glm::translate(model, glm::vec3(star.x, star.y, -14.9f));

    abcg::glUniform1f(ul_opacity, static_cast<float>(star.z));
    abcg::glUniformMatrix4fv(ul_model_matrix, 1, GL_FALSE, &model[0][0]);
    abcg::glUniformMatrix4fv(ul_view_matrix, 1, GL_FALSE,
                             &(*m_viewMatrix)[0][0]);
    abcg::glUniformMatrix4fv(ul_proj_matrix, 1, GL_FALSE,
                             &(*m_projMatrix)[0][0]);

    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 8 + 2);
  }

  glDisable(GL_BLEND);
  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Stars::terminate() {
  abcg::glDeleteBuffers(1, &vbo);
  abcg::glDeleteVertexArrays(1, &vao);
}

void Stars::update(float deltaTime) {
  auto &re{random_engine};
  std::uniform_real_distribution<float> dist_pos_x(pos_x_min, pos_x_max);
  std::uniform_real_distribution<float> dist_pos_y(pos_y_min, pos_y_max);
  for (auto &star : stars) {
    star.z += 0.1f * deltaTime;
    if (star.z >= 1.0) {
      star.z = 0;
      star.x = dist_pos_x(re);
      star.y = dist_pos_y(re);
    }
  }
}
