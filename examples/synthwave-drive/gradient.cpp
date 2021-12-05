#include "gradient.hpp"

void Gradient::initialize(GLuint _program) {
  program = _program;

  std::array vertices{glm::vec2(50.0f, 0.0f), glm::vec2(50.0f, 12.0f),
                      glm::vec2(-50.0f, 12.0f), glm::vec2(-50.0, -0.0f)};

  std::array colors{glm::vec4(0.976f, 0.161f, 0.578f, 0.9f),
                    glm::vec4(0.976f, 0.161f, 0.578f, -0.15f),
                    glm::vec4(0.976f, 0.161f, 0.578f, -0.15f),
                    glm::vec4(0.976f, 0.161f, 0.578f, 0.9f)};

  abcg::glGenBuffers(1, &vbo_position);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenBuffers(1, &vbo_color);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
  abcg::glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4),
                     colors.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenVertexArrays(1, &vao);
  abcg::glBindVertexArray(vao);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
  const GLint posAttrib{abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(posAttrib);
  abcg::glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  const GLint colorAttrib{abcg::glGetAttribLocation(program, "inColor")};
  abcg::glEnableVertexAttribArray(colorAttrib);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
  abcg::glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindVertexArray(0);

  ul_view_matrix = abcg::glGetUniformLocation(program, "viewMatrix");
  ul_proj_matrix = abcg::glGetUniformLocation(program, "projMatrix");
  ul_model_matrix = abcg::glGetUniformLocation(program, "modelMatrix");
}

void Gradient::paint(glm::mat4* m_viewMatrix, glm::mat4* m_projMatrix) const {
  abcg::glUseProgram(program);
  abcg::glBindVertexArray(vao);

  glm::mat4 model = glm::mat4{1};
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -14.7f));
  abcg::glUniformMatrix4fv(ul_model_matrix, 1, GL_FALSE, &model[0][0]);
  abcg::glUniformMatrix4fv(ul_view_matrix, 1, GL_FALSE, &(*m_viewMatrix)[0][0]);
  abcg::glUniformMatrix4fv(ul_proj_matrix, 1, GL_FALSE, &(*m_projMatrix)[0][0]);

  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glDisable(GL_BLEND);
  abcg::glUseProgram(0);
}

void Gradient::terminate() {
  abcg::glDeleteBuffers(1, &vbo_position);
  abcg::glDeleteBuffers(1, &vbo_color);
  abcg::glDeleteVertexArrays(1, &vao);
}
