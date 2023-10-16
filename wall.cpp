#include "wall.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Wall::create(GLuint program) {
  destroy();

  m_program = program;

  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_translation = glm::vec2(0);

  m_wallArrays = {
    glm::vec4{0.0f, 0.33f, -0.33f, 0.33f},
    glm::vec4{-0.33f, 0.33f, -0.33f, 0.0f},
    glm::vec4{ -0.33f, 0.0f,    0.66f, 0.0f},
    glm::vec4{ 0.66f, 0.0f,    0.66f, 0.33f},
    glm::vec4{ 0.66f, 0.33f,    0.66f, 0.66f},
    glm::vec4{ 0.66f, 0.33f,    1.0f, 0.33f},
    glm::vec4{ 1.0f, 0.33f,    1.0f, 1.0f},
    glm::vec4{ 1.0f, 1.0f,    -0.33f, 1.0f},
    glm::vec4{ -0.33f, 1.0f,    -0.33f, 0.66f},
    glm::vec4{ -0.33f, 0.66f,    0.33f, 0.66f},
    glm::vec4{ 0.33f, 0.66f,    0.33f, 0.33f},
    glm::vec4{ -0.33f, 0.0f,    -0.33f, -0.66f},
    glm::vec4{ -0.33f, -0.66f,    -0.66f, -0.66f},
    glm::vec4{ -0.66f, -0.33f,    -0.66f, -0.66f},
    glm::vec4{ 0.66f, 0.0f,    0.66f, -0.33f},
    glm::vec4{ 0.66f, -0.33f,    0.33f, -0.33f},
    glm::vec4{ -0.66f, 0.66f,    -0.66f, 1.0f},
    glm::vec4{ -0.66f, 1.0f,    -1.0f, 1.0f},
    glm::vec4{ -1.0f, 1.0f,    -1.0f, 0.33f},
    glm::vec4{ -1.0f, 0.33f,    -0.66f, 0.33f},
    glm::vec4{ -0.66f, 0.33f,    -0.66f, 0.0f},
    glm::vec4{ -1.0f, 0.33f,    -1.0f, -1.0f},
    glm::vec4{ -1.0f, -1.0f,    0.0f, -1.0f},
    glm::vec4{ 0.0f, -1.0f,    0.0f, -0.33f},
    glm::vec4{ 0.0f, -1.0f,    0.33f, -1.0f},
    glm::vec4{ 0.33f, -1.0f,    0.33f, -0.66f},
    glm::vec4{ 0.33f, -0.66f,    0.66f, -0.66f},
    glm::vec4{ 0.33f, -1.0f,    1.0f, -1.0f},
    glm::vec4{ 1.0f, -1.0f,    1.0f, 0.0f}, 
  };

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_wallArrays), m_wallArrays.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

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

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Wall::paint(const GameData &gameData) {
  if (gameData.m_state != State::Playing)
    return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawArrays(GL_LINES, 0, 100);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Wall::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}
