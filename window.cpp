#include "window.hpp"

void Window::onEvent(SDL_Event const &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Right));
  }
}

void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Load a new font
  auto const filename{assetsPath + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError("Cannot load font file");
  }

  // Create program to render the other objects
  m_cube_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "laby.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "laby.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  restart();
}

void Window::restart() {
  m_gameData.m_state = State::Playing;
  m_cube.create(m_cube_program);
  m_wall.create(m_cube_program);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }


// TODO
  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
    checkWinCondition();
  }

  m_cube.update(m_gameData, deltaTime);
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  m_cube.paint(m_gameData);
  m_wall.paint(m_gameData);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto const size{ImVec2(300, 85)};
    auto const position{ImVec2((m_viewportSize.x - size.x) / 2.0f,
                               (m_viewportSize.y - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags const flags{ImGuiWindowFlags_NoBackground |
                                 ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::Finished) {
      ImGui::Text("Finished!");
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  m_wall.destroy();
  m_cube.destroy();
}

void Window::checkCollisions() {
  m_gameData.m_collision = Collision::False;
  m_gameData.m_collisionSide1 = 0;
  m_gameData.m_collisionSide2 = 0;
  for (auto i : iter::range(m_cube.m_cubeSidePoints.size())) {
    auto cubeSidePoints = m_cube.m_cubeSidePoints[i];

    for (auto wall : m_wall.m_wallArrays) {
      if(isPointInLine((wall*m_wall.m_scale), (cubeSidePoints*m_cube.m_scale + m_cube.m_translation))) {
        m_gameData.m_collision = Collision::True;
        if (m_gameData.m_collisionSide1 == 0)
          m_gameData.m_collisionSide1 = i+1;
        else 
          m_gameData.m_collisionSide2 = i+1;
        
      }
    }
  }
  
}

bool Window::isPointInLine(glm::vec4 line, glm::vec2 point) { 

  auto const distLineStartPoint {
    sqrtf(powf((line.x - point.x),2.0) + powf((line.y - point.y),2.0))
  };

  auto const distLineEndPoint {
    sqrtf(powf((line.z - point.x),2.0) + powf((line.w - point.y),2.0))
  };

  auto const lineLength {
    sqrtf(powf((line.x - line.z),2.0) + powf((line.y - line.w),2.0))
  };
  auto const distanceDiff { (distLineStartPoint + distLineEndPoint) - lineLength};

  return distanceDiff < 0.001; 
}

void Window::checkWinCondition() {
  for (int i : iter::range(m_cube.m_cubeSidePoints.size())) {
    auto cubeSidePoints = m_cube.m_cubeSidePoints[i];

    for (auto winningWall : m_wall.m_winningArea) {
      if(isPointInLine((winningWall*m_wall.m_scale), (cubeSidePoints*m_cube.m_scale + m_cube.m_translation))) {
          m_gameData.m_state = State::Finished;
          m_restartWaitTimer.restart();

        return;
      }
    }
  }
}

