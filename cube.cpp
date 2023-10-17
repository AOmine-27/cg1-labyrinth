#include "cube.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Cube::create(GLuint program) {
  destroy();

  m_program = program;

  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_translation = glm::vec2(-0.3, 0.5);
  m_velocity = glm::vec2(0);

  m_cubeSidePoints = {
      glm::vec2{0.0f, +0.5f}, 
      glm::vec2{0.5f, 0.0f},
      glm::vec2{+1.0f, +0.5f}, 
      glm::vec2{+0.5f, 1.0f},
      };

  std::array const indices{ 0, 1, 2, 0, 3, 2};

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_cubeSidePoints), m_cubeSidePoints.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Cube::paint(const GameData &gameData) {
  if (gameData.m_state != State::Playing)
    return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Cube::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Cube::update(GameData const &gameData, float deltaTime) {
  auto vel{0.5f};
  // TODO: Check if hit borders/walls
  if (gameData.m_collision == Collision::True) {
    auto side1 = gameData.m_collisionSide1;
    auto side2 = gameData.m_collisionSide2;
    

    if (gameData.m_input[gsl::narrow<size_t>(Input::Left)] && (side1 != 1 && side2 != 1))
      m_translation -= glm::vec2{0.2f, 0.0f} * deltaTime*vel;
    if (gameData.m_input[gsl::narrow<size_t>(Input::Right)]  && (side1 != 3 && side2 != 3))
      m_translation += glm::vec2{0.2f, 0.0f} * deltaTime*vel;
    if (gameData.m_input[gsl::narrow<size_t>(Input::Up)]  && (side1 != 4 && side2 != 4))
      m_translation += glm::vec2{0.0f, 0.2f} * deltaTime*vel;
    if (gameData.m_input[gsl::narrow<size_t>(Input::Down)]  && (side1 != 2 && side2 != 2))
      m_translation -= glm::vec2{0.0f, 0.2f} * deltaTime*vel;

  } else {
    if (gameData.m_input[gsl::narrow<size_t>(Input::Left)])
      m_translation -= glm::vec2{0.2f, 0.0f} * deltaTime*vel;
    if (gameData.m_input[gsl::narrow<size_t>(Input::Right)])
      m_translation += glm::vec2{0.2f, 0.0f} * deltaTime*vel;
    if (gameData.m_input[gsl::narrow<size_t>(Input::Up)])
      m_translation += glm::vec2{0.0f, 0.2f} * deltaTime*vel;
    if (gameData.m_input[gsl::narrow<size_t>(Input::Down)])
      m_translation -= glm::vec2{0.0f, 0.2f} * deltaTime*vel;
  }

}