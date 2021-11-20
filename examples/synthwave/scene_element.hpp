#ifndef SCENE_ELEMENT
#define SCENE_ELEMENT

#include "abcg.hpp"

class SceneElement {
 public:
  SceneElement() = default;
  virtual ~SceneElement() = default;

  SceneElement(const SceneElement&) = default;
  SceneElement(SceneElement&&) = default;
  SceneElement& operator=(const SceneElement&) = default;
  SceneElement& operator=(SceneElement&&) = default;

  virtual void initialize(GLuint program) = 0;
  virtual void update(float /*unused*/) {};
  virtual void paint() = 0;
  virtual void terminate() = 0;
};

#endif
