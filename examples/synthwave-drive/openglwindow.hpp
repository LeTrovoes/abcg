#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <bitset>

#include "abcg.hpp"
#include "car.hpp"
#include "gradient.hpp"
#include "ground.hpp"
#include "model.hpp"
#include "stars.hpp"
#include "sun.hpp"

enum class Input { Right, Left };

using PlayerInput = std::bitset<2>;

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint program{};
  GLuint program_texture{};
  GLuint program_blinnphong{};
  GLuint program_blur{};

  GLuint blur_vbo{};
  GLuint blur_vao{};
  GLuint glowFBO;
  GLuint glowTexture;

  GLuint intermediateFBO;
  GLuint intermediateTexture;

  int viewport_width{1024};
  int viewport_height{576};

  Car car;
  Sun sun;
  Stars stars;
  Model model;
  Ground ground;
  Gradient gradient;

  GLint ul_view_matrix{};
  GLint ul_proj_matrix{};
  GLint ul_model_matrix{};
  GLint ul_normal_matrix{};
  GLint ul_light_dir{};
  GLint ul_shininess{};
  GLint ul_Ia{};
  GLint ul_Id{};
  GLint ul_Is{};
  GLint ul_Ka{};
  GLint ul_Kd{};
  GLint ul_Ks{};
  GLint ul_diffuseTex{};
  GLint ul_mappingMode{};
  GLint ul_color{};
  GLint ul_shadow_range{};
  GLint ul_shadow_offset{};

  glm::mat4 modelMatrixCar{1.0f};
  glm::mat4 viewMatrix{1.0f};
  glm::mat4 projMatrix{1.0f};

  std::vector<glm::mat4> palmTreeModelMatrixes;

  float car_position{0};

  PlayerInput input;

  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka;
  glm::vec4 m_Kd;
  glm::vec4 m_Ks;
  float m_shininess{};

  void loadModel(std::string_view path);
  void update();
};

#endif
