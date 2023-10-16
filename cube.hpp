#ifndef CUBE_HPP_
#define CUBE_HPP_

#include "abcgOpenGL.hpp"

#include "gamedata.hpp"

class Cube {
public:
  void create(GLuint program);
  void paint(GameData const &gameData);
  void destroy();
  void update(GameData const &gameData, float deltaTime);

  glm::vec4 m_color{1};
  float m_scale{0.1f};
  glm::vec2 m_translation{};
  glm::vec2 m_velocity{};
  std::array<glm::vec2, 4> m_cubeSidePoints{};

private:
  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
};
#endif