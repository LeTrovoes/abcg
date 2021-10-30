#include "bullets.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "core.h"

void Bullets::initializeGL(GLuint program) {
  terminateGL();

  gl_program = program;
  ul_color = abcg::glGetUniformLocation(gl_program, "color");
  ul_rotation = abcg::glGetUniformLocation(gl_program, "rotation");
  ul_scale = abcg::glGetUniformLocation(gl_program, "scale");
  ul_translation = abcg::glGetUniformLocation(gl_program, "translation");

  bullets.clear();

  // Create regular polygon
  const auto sides{10};

  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  const auto step{M_PI * 2 / sides};
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO of positions
  abcg::glGenBuffers(1, &gl_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     static_cast<int>(positions.size() * sizeof(glm::vec2)),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  const GLint positionAttribute{
      abcg::glGetAttribLocation(gl_program, "inPosition")};

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

void Bullets::paintGL() {
  abcg::glUseProgram(gl_program);

  abcg::glBindVertexArray(gl_vao);
  abcg::glUniform4f(ul_color, 1, 1, 1, 1);
  abcg::glUniform1f(ul_rotation, 0);
  abcg::glUniform1f(ul_scale, scale);

  for (const auto &bullet : bullets) {
    abcg::glUniform2f(ul_translation, bullet.translation.x,
                      bullet.translation.y);

    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
  }

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Bullets::terminateGL() {
  abcg::glDeleteBuffers(1, &gl_vbo);
  abcg::glDeleteVertexArrays(1, &gl_vao);
}

void Bullets::update(Ship &ship, GameData &gameData, float deltaTime, PlayerName player) {
  // Create a pair of bullets

  auto batata = player == PlayerName::Red ? gameData.red_input : gameData.blu_input;
  if (batata[static_cast<size_t>(Input::Fire)] &&
      gameData.state == State::Playing) {
    // At least 250 ms must have passed since the last bullets
    if (ship.bulletCoolDownTimer.elapsed() > 250.0 / 1000.0) {
      ship.bulletCoolDownTimer.restart();

      // Bullets are shot in the direction of the ship's forward vector
      glm::vec2 forward{glm::rotate(glm::vec2{0.0f, 1.0f}, ship.rotation)};
      // glm::vec2 right{glm::rotate(glm::vec2{1.0f, 0.0f}, ship.m_rotation)};
      // const auto cannonOffset{(11.0f / 15.5f) * ship.m_scale};
      const auto bulletSpeed{2.0f};

      Bullet bullet{.dead = false,
                    .translation = ship.translation,
                    .velocity = ship.velocity + forward * bulletSpeed};
      bullets.push_back(bullet);

      // Moves ship in the opposite direction
      /*ship.m_velocity -= forward * 0.1f;*/
    }
  }

  for (auto &bullet : bullets) {
    bullet.translation -= ship.velocity * deltaTime;
    bullet.translation += bullet.velocity * deltaTime;

    // Kill bullet if it goes off screen
    if (bullet.translation.x < -1.1f) bullet.dead = true;
    if (bullet.translation.x > +1.1f) bullet.dead = true;
    if (bullet.translation.y < -1.1f) bullet.dead = true;
    if (bullet.translation.y > +1.1f) bullet.dead = true;
  }

  // Remove dead bullets
  bullets.remove_if([](const Bullet &b) { return b.dead; });
}
