#ifndef ROSA_HPP_
#define ROSA_HPP_

#include "abcg.hpp"

class Rosa {
 public:
  void initializeGL(GLuint _program);
  void paintGL(glm::mat4* m_viewMatrix, glm::mat4* m_projMatrix);
  void terminateGL();

 private:
  GLuint m_program{};
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint oto_VBO{};

};

#endif
