//#include <glm/gtx/fast_trigonometry.hpp>
//#include <glm/gtx/rotate_vector.hpp>

#include "health.hpp"

void Health::initializeGL(GLuint _program) {
  terminateGL();

  program = _program;
  colorLoc = abcg::glGetUniformLocation(program, "color");
  rotationLoc = abcg::glGetUniformLocation(program, "rotation");
  scaleLoc = abcg::glGetUniformLocation(program, "scale");
  translationLoc = abcg::glGetUniformLocation(program, "translation");

  translation = glm::vec2(0,0.9);

  std::array<glm::vec2, 58> positions{
      // Heart body
      glm::vec2{+3.0f, +9.0f},  glm::vec2{+4.0f, +9.0f},
      glm::vec2{+8.0f, +9.0f},  glm::vec2{+9.0f, +9.0f},
      glm::vec2{+2.0f, +8.0f},  glm::vec2{+3.0f, +8.0f},
      glm::vec2{+4.0f, +8.0f},  glm::vec2{+5.0f, +8.0f},
      glm::vec2{+7.0f, +8.0f},  glm::vec2{+8.0f, +8.0f},
      glm::vec2{+9.0f, +8.0f},  glm::vec2{+10.0f, +8.0f},
      glm::vec2{+1.0f, +7.0f},  glm::vec2{+2.0f, +7.0f},
      glm::vec2{+3.0f, +7.0f},  glm::vec2{+4.0f, +7.0f},
      glm::vec2{+5.0f, +7.0f},  glm::vec2{+7.0f, +7.0f},
      glm::vec2{+8.0f, +7.0f},  glm::vec2{+9.0f, +7.0f},
      glm::vec2{+10.0f, +7.0f}, glm::vec2{+1.0f, +6.0f},
      glm::vec2{+2.0f, +6.0f},  glm::vec2{+3.0f, +6.0f},
      glm::vec2{+4.0f, +6.0f},  glm::vec2{+5.0f, +6.0f},
      glm::vec2{+6.0f, +6.0f},  glm::vec2{+7.0f, +6.0f},
      glm::vec2{+8.0f, +6.0f},  glm::vec2{+9.0f, +6.0f},
      glm::vec2{+10.0f, +6.0f}, glm::vec2{+11.0f, +6.0f},
      glm::vec2{+2.0f, +5.0f},  glm::vec2{+3.0f, +5.0f},
      glm::vec2{+4.0f, +5.0f},  glm::vec2{+5.0f, +5.0f},
      glm::vec2{+6.0f, +5.0f},  glm::vec2{+7.0f, +5.0f},
      glm::vec2{+8.0f, +5.0f},  glm::vec2{+9.0f, +5.0f},
      glm::vec2{+10.0f, +5.0f}, glm::vec2{+3.0f, +4.0f},
      glm::vec2{+4.0f, +4.0f},  glm::vec2{+5.0f, +4.0f},
      glm::vec2{+6.0f, +4.0f},  glm::vec2{+7.0f, +4.0f},
      glm::vec2{+8.0f, +4.0f},  glm::vec2{+9.0f, +4.0f},
      glm::vec2{+4.0f, +3.0f},  glm::vec2{+5.0f, +3.0f},
      glm::vec2{+6.0f, +3.0f},  glm::vec2{+7.0f, +3.0f},
      glm::vec2{+8.0f, +3.0f},  glm::vec2{+5.0f, +2.0f},
      glm::vec2{+6.0f, +2.0f},  glm::vec2{+7.0f, +2.0f},
      glm::vec2{+6.0f, +1.0f},  glm::vec2{+11.0f, +7.0f},
  };

  // Normalize
  for (auto &position : positions) {
    position /= glm::vec2{11.0f, 11.0f};
  }

  // Generate VBO
  abcg::glGenBuffers(1, &vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Health::paintGL(int placement, PlayerName player) {
  abcg::glUseProgram(program);

  abcg::glBindVertexArray(vao);

  auto offset = player == PlayerName::Red ? -0.75 : 0.5;
  offset +=placement * 0.1;

  auto health_position = glm::vec2(translation.x + offset, translation.y);

  abcg::glUniform1f(scaleLoc, scale);
  abcg::glUniform1f(rotationLoc, rotation);
  abcg::glUniform2fv(translationLoc, 1, &health_position.x);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
  abcg::glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
  abcg::glDrawArrays(GL_POINTS, 0, 58);
#if !defined(__EMSCRIPTEN__)
  abcg::glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Health::terminateGL() {
  abcg::glDeleteBuffers(1, &vbo);
  abcg::glDeleteVertexArrays(1, &vao);
}
