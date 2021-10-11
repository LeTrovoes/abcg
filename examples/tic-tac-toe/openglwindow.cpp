#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

#include "imgui_internal.h"

void OpenGLWindow::initializeGL() {
  game->Restart();
  auto windowSettings{getWindowSettings()};
  fmt::print("Initial window size: {}x{}\n", windowSettings.width,
             windowSettings.height);
}

void OpenGLWindow::paintGL() {
  // Set the clear color
  abcg::glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2],
                     m_clearColor[3]);
  // Clear the color buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::paintUI() {
  // Parent class will show fullscreen button and FPS meter
  abcg::OpenGLWindow::paintUI();

  // Our own ImGui widgets go below
  {
    // Window begin
    auto flags = ImGuiWindowFlags_NoResize;
    ImGui::Begin("Tic-Tac-Toe", nullptr, flags);

    // Game status
    if (game->isPlaying()) {
      ImGui::Text("%c turn", game->getTurn());
    } else if (game->isDraw()) {
      ImGui::Text("Draw!");
    } else {
      ImGui::Text("%c is the winner", game->getWinner());
    }
    ImGui::Spacing();

    // Board
    if (ImGui::BeginTable("tictactoe", 3)) {
      for (int i = 0; i < 9; i++) {
        ImGui::TableNextColumn();
        ImGui::Button(std::string{game->getValueAt(i)}.c_str(), ImVec2(100, 100));
        if (i < 6) ImGui::Spacing();

        if (ImGui::IsItemClicked()) {
          game->Select(i);
        }
      }

      ImGui::EndTable();
    }

    // Restart button
    ImGui::Spacing();
    ImGui::Button("Restart", ImVec2(-1, 50));
    if (ImGui::IsItemClicked()) {
      game->Restart();
    }

    // Window end
    ImGui::End();
  }
}
