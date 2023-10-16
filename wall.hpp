#ifndef WALL_HPP_
#define WALL_HPP_

#include "abcgOpenGL.hpp"

#include "gamedata.hpp"

class Wall {
public:
  void create(GLuint program);
  void paint(GameData const &gameData);
  void destroy();

  glm::vec4 m_color{1};
  float m_scale{0.5f};
  glm::vec2 m_translation{};
  glm::vec2 m_velocity{};
  std::array<glm::vec4, 29> m_wallArrays{};
  std::array<glm::vec4, 3> m_winningArea{glm::vec4{1.0f, 0.33f, 1.2f, 0.33f},
                                         glm::vec4{1.2f, 0.33f, 1.2f, 0.0f},
                                         glm::vec4{1.2f, 0.0f, 1.0f, 0.0f}};


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