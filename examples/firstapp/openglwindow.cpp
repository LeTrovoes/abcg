#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include <vector>

void OpenGLWindow::initializeGL() {
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
    ImGui::Begin("Hello, First App!");

    // Static text
    auto windowSettings{getWindowSettings()};
    ImGui::Text("Current window size: %dx%d (in windowed mode)",
                windowSettings.width, windowSettings.height);

    // Slider from 0.0f to 1.0f
    static float f{};
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

    // ColorEdit to change the clear color
    ImGui::ColorEdit3("clear color", m_clearColor.data());

    // More static text
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // 100x50 button
    if (ImGui::Button("Press me!", ImVec2(100, 100))) {
      fmt::print("Button pressed.\n");
    }

    /*

    // Nx50 button, where N is the remaining width available
    ImGui::Button("Press me!", ImVec2(-1, 50));
    // See also IsItemHovered, IsItemActive, etc
    if (ImGui::IsItemClicked()) {
      fmt::print("Button pressed.\n");
    }

    static bool enabled{true};
    ImGui::Checkbox("Some option", &enabled);
    fmt::print("The checkbox is {}\n", enabled ? "enabled" : "disabled");

    static std::size_t currentIndex{};
    std::vector<std::string> comboItems{"AAA", "BBB", "CCC"};

    if (ImGui::BeginCombo("Combo box", comboItems.at(currentIndex).c_str()))
   { for (auto index{0u}; index < comboItems.size(); ++index) { const bool
   isSelected{currentIndex == index}; if
   (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
          currentIndex = index;

        // Set the initial focus when opening the combo (scrolling +
   keyboard
        // navigation focus)
        if (isSelected) ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }

    fmt::print("Selected combo box item: {}\n",
   comboItems.at(currentIndex)); */

    // Window end
    ImGui::End();
  }
}
