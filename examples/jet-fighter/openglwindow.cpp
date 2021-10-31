#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <vector>

#include "bullets.hpp"
#include "core.h"
#include "gamedata.hpp"

void OpenGLWindow::initializeGL() {
  ImGuiIO &io{ImGui::GetIO()};
  const auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  auto windowSettings{getWindowSettings()};
  fmt::print("Initial window size: {}x{}\n", windowSettings.width,
             windowSettings.height);

  gl_objects_program = createProgramFromFile(getAssetsPath() + "objects.vert",
                                             getAssetsPath() + "objects.frag");
  healthProgram = createProgramFromFile(getAssetsPath() + "coracao.vert",
                                        getAssetsPath() + "objects.frag");

  abcg::glClearColor(0, 0, 0, 1);

  health.initializeGL(healthProgram);

  red_ship.player_name = PlayerName::Red;
  red_bullets.initializeGL(gl_objects_program);
  red_ship.initializeGL(gl_objects_program);

  blu_ship.player_name = PlayerName::Blu;
  blu_bullets.initializeGL(gl_objects_program);
  blu_ship.initializeGL(gl_objects_program);

  restart();
}

void OpenGLWindow::paintGL() {
  update();
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  red_bullets.paintGL();
  if (red_ship.life > 0) {
    red_ship.paintGL();
  }

  blu_bullets.paintGL();
  if (blu_ship.life > 0) {
    blu_ship.paintGL();
  }

  health.color = red_ship.color;
  for (int x = 0; x < red_ship.life; x++) {
    health.paintGL(x, PlayerName::Red);
  }

  health.color = blu_ship.color;
  for (int x = 0; x < blu_ship.life; x++) {
    health.paintGL(x, PlayerName::Blu);
  }
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    const auto size{ImVec2(300, 300)};
    const auto position{ImVec2((viewport_width - size.x) / 2.0f,
                               (viewport_height - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar};

    if (gameData.state == State::Playing) {
      flags |= ImGuiWindowFlags_NoInputs;
    }

    ImGui::Begin("Window", nullptr, flags);
    ImGui::PushFont(font);

    if (gameData.state == State::Draw) {
      ImGui::SameLine(90);
      ImGui::Text("Draw!");
    } else if (gameData.state == State::Winner_Red) {
      ImGui::SameLine(30);
      ImGui::Text("Red Wins!");
    } else if (gameData.state == State::Winner_Blu) {
      ImGui::Text("Blue Wins!");
    }

    ImGui::PopFont();

    if (gameData.state != State::Playing) {
      ImGui::Dummy(ImVec2(0, 100));
      ImGui::Button("Restart", ImVec2(300, 50));
      if (ImGui::IsItemClicked()) {
        restart();
      }
    }

    ImGui::End();
  }
}

void OpenGLWindow::update() {
  if (gameData.state != State::Playing) return;

  const float deltaTime{static_cast<float>(getDeltaTime())};

  red_bullets.update(red_ship, gameData, deltaTime, PlayerName::Red);
  blu_bullets.update(blu_ship, gameData, deltaTime, PlayerName::Blu);

  red_ship.update(gameData.red_input, deltaTime);
  blu_ship.update(gameData.blu_input, deltaTime);

  checkColisions();
  checkWinners();
}

void OpenGLWindow::checkColisions() {
  /* Check if player1 hit player2 */
  for (auto &bullet : red_bullets.bullets) {
    auto distance = glm::distance(bullet.translation, blu_ship.translation);
    if (distance < blu_ship.scale * 1.1f) {
      blu_ship.hit();
      bullet.dead = true;
    }
  }

  /* Check if player1 hit player2 */
  for (auto &bullet : blu_bullets.bullets) {
    auto distance = glm::distance(bullet.translation, red_ship.translation);
    if (distance < red_ship.scale * 1.1f) {
      red_ship.hit();
      bullet.dead = true;
    }
  }

  /* Check if both players colidded */
  {
    auto distance = glm::distance(red_ship.translation, blu_ship.translation);
    if (distance < red_ship.scale * 0.7f + blu_ship.scale * 0.7f) {
      red_ship.hit();
      blu_ship.hit();
    }
  }
}

void OpenGLWindow::checkWinners() {
  if (red_ship.life == 0 && blu_ship.life == 0) {
    gameData.state = State::Draw;
  } else if (red_ship.life <= 0) {
    gameData.state = State::Winner_Blu;
  } else if (blu_ship.life <= 0) {
    gameData.state = State::Winner_Red;
  }
}

void OpenGLWindow::handleEvent(SDL_Event &event) {
  if (event.type == SDL_KEYDOWN) {
    // Player 01
    if (event.key.keysym.sym == SDLK_w)
      gameData.red_input.set(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_a)
      gameData.red_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_d)
      gameData.red_input.set(static_cast<size_t>(Input::Right));

    // Player 02
    if (event.key.keysym.sym == SDLK_UP)
      gameData.blu_input.set(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_LEFT)
      gameData.blu_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT)
      gameData.blu_input.set(static_cast<size_t>(Input::Right));
  }

  if (event.type == SDL_KEYUP) {
    // Player 01
    if (event.key.keysym.sym == SDLK_w)
      gameData.red_input.reset(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_a)
      gameData.red_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_d)
      gameData.red_input.reset(static_cast<size_t>(Input::Right));

    // Player 02
    if (event.key.keysym.sym == SDLK_UP)
      gameData.blu_input.reset(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_LEFT)
      gameData.blu_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT)
      gameData.blu_input.reset(static_cast<size_t>(Input::Right));
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  viewport_width = width;
  viewport_height = height;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::restart() {
  gameData.red_input.reset();
  gameData.blu_input.reset();

  red_ship.restart();
  blu_ship.restart();

  red_bullets.restart();
  blu_bullets.restart();

  gameData.state = State::Playing;
}
