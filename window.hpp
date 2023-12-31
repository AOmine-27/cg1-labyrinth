#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"
#include "cube.hpp"
#include "wall.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_cube_program{};

  GameData m_gameData;

  Cube m_cube;
  Wall m_wall;

  abcg::Timer m_restartWaitTimer;

  ImFont *m_font{};

  void restart();
  void checkCollisions();
  bool isPointInLine(glm::vec4 line, glm::vec2 point);
  void checkWinCondition();
};

#endif