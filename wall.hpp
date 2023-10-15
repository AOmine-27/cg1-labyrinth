#ifndef WALL_HPP_
#define WALL_HPP_

#include "abcgOpenGL.hpp"

#include "gamedata.hpp"

class Wall {
public:
  void create(GLuint program);
  void paint(GameData const &gameData);
  void destroy();
  void update(GameData const &gameData, float deltaTime);

  glm::vec4 m_color{1};
  float m_rotation{};
  float m_scale{0.5f};
  glm::vec2 m_translation{};
  glm::vec2 m_velocity{};

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