#include "ship.hpp"

#include <cmath>
#include <glm/fwd.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "gamedata.hpp"

void Ship::initializeGL(GLuint _program) {
  terminateGL();

  gl_program = _program;
  ul_color = abcg::glGetUniformLocation(gl_program, "color");
  ul_rotation = abcg::glGetUniformLocation(gl_program, "rotation");
  ul_scale = abcg::glGetUniformLocation(gl_program, "scale");
  ul_translation = abcg::glGetUniformLocation(gl_program, "translation");

  color = player_number == 1 ? glm::vec4{1, 0, 0, 1} : glm::vec4{0, 0.4, 1, 1};

  std::array<glm::vec2, 4> positions{
      // Ship body
      glm::vec2{+0.0f, +0.5f},
      glm::vec2{+0.5f, -0.5f},
      glm::vec2{+0.0f, -0.25f},
      glm::vec2{-0.5f, -0.5f},
  };

  // Normalize
  for (auto &position : positions) {
    position /= glm::vec2{0.5f, 0.5f};
  }

  // Generate VBO
  abcg::glGenBuffers(1, &gl_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(gl_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &gl_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(gl_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Ship::restart() {
    life = 3;
    translation = player_number == 1 ? glm::vec2(-0.5, 0) : glm::vec2(0.5, 0);
    rotation = 0.0f;
    velocity = glm::vec2(0);
    isImmune = false;
    immunityTimer.restart();
    bulletCoolDownTimer.restart();
}

void Ship::paintGL() {
  abcg::glUseProgram(gl_program);

  abcg::glBindVertexArray(gl_vao);

  abcg::glUniform1f(ul_scale, scale);
  abcg::glUniform1f(ul_rotation, rotation);
  abcg::glUniform2fv(ul_translation, 1, &translation.x);

  if (isImmune && immunityTimer.elapsed() < 2) {
    if (fmod(immunityTimer.elapsed(), 0.1) < 0.05) {
      return;
    }
  }

  abcg::glUniform4f(ul_color, color.r, color.g, color.b, color.a);

  for (auto i : {-2, 0, 2}) {
    for (auto j : {-2, 0, 2}) {
      abcg::glUniform2f(ul_translation, translation.x + j, translation.y + i);
      abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
  }

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}

void Ship::terminateGL() {
  abcg::glDeleteBuffers(1, &gl_vbo);
  abcg::glDeleteVertexArrays(1, &gl_vao);
}

void Ship::hit() {
  if (isImmune) return;
  life--;
  isImmune = true;
  immunityTimer.restart();
}

void Ship::update(const GameData &gameData, float deltaTime) {
  auto input = player_number == 1 ? gameData.red_input : gameData.blu_input;
  if (gameData.state == State::Playing) {
    if (input[static_cast<size_t>(Input::Left)])
      rotation = glm::wrapAngle(rotation + 4.0f * deltaTime);
    if (input[static_cast<size_t>(Input::Right)])
      rotation = glm::wrapAngle(rotation - 4.0f * deltaTime);

    glm::vec2 forward = glm::rotate(glm::vec2{0.0f, 1.0f}, rotation);
    velocity = forward * 0.5f;
    translation += velocity * deltaTime;
  }

  // Wrap-around
  if (translation.x < -1.0f) translation.x += 2.0f;
  if (translation.x > +1.0f) translation.x -= 2.0f;
  if (translation.y < -1.0f) translation.y += 2.0f;
  if (translation.y > +1.0f) translation.y -= 2.0f;

  if (immunityTimer.elapsed() > 2) {
    isImmune = false;
  }
}
