#ifndef CAR_HPP_
#define CAR_HPP_

#include <vector>

#include "abcg.hpp"
#include "vertex.hpp"

class Car {
 public:
  void loadDiffuseTexture(std::string_view path);
  void loadObj(std::string_view path, bool standardize = true);
  void render(int numTriangles = -1) const;
  void setupVAO(GLuint program);
  void terminateGL();

  [[nodiscard]] int getNumTriangles() const {
    return static_cast<int>(indexes.size()) / 3;
  }

  [[nodiscard]] glm::vec4 getKa() const { return m_Ka; }
  [[nodiscard]] glm::vec4 getKd() const { return m_Kd; }
  [[nodiscard]] glm::vec4 getKs() const { return m_Ks; }
  [[nodiscard]] float getShininess() const { return m_shininess; }

  [[nodiscard]] bool isUVMapped() const { return m_hasTexCoords; }

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  glm::vec4 m_Ka;
  glm::vec4 m_Kd;
  glm::vec4 m_Ks;
  float m_shininess;
  GLuint m_diffuseTexture{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> indexes;

  bool m_hasNormals{false};
  bool m_hasTexCoords{false};

  void computeNormals();
  void createBuffers();
  void standardize();
};

#endif