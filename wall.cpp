#include "wall.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Wall::create(GLuint program) {
  destroy();

  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Reset ship attributes
  m_translation = glm::vec2(0);
  m_velocity = glm::vec2(0);

  // clang-format off
  std::array positions{
    glm::vec2{0.0f, 0.33f}, 
    glm::vec2{-0.33f, 0.33f},

    glm::vec2{-0.33f, 0.33f},
    glm::vec2{-0.33f, 0.0f},

    glm::vec2{-0.33f, 0.0f},
    glm::vec2{0.66f, 0.0f},

    glm::vec2{0.66f, 0.0f},
    glm::vec2{0.66f, 0.33f},

    glm::vec2{0.66f, 0.33f},
    glm::vec2{0.66f, 0.66f},

    glm::vec2{0.66f, 0.33f},
    glm::vec2{1.0f, 0.33f},

    glm::vec2{1.0f, 0.33f},
    glm::vec2{1.0f, 1.0f},

    glm::vec2{1.0f, 1.0f},
    glm::vec2{-0.33f, 1.0f},

    glm::vec2{-0.33f, 1.0f},
    glm::vec2{-0.33f, 0.66f},

    glm::vec2{-0.33f, 0.66f},
    glm::vec2{0.33f, 0.66f},

    glm::vec2{0.33f, 0.66f},
    glm::vec2{0.33f, 0.33f},

    glm::vec2{-0.33f, 0.0f},
    glm::vec2{-0.33f, -0.66f},

    glm::vec2{-0.33f, -0.66f},
    glm::vec2{-0.66f, -0.66f},

    glm::vec2{-0.66f, -0.33f},
    glm::vec2{-0.66f, -0.66f},

    glm::vec2{0.66f, 0.0f},
    glm::vec2{0.66f, -0.33f},

    glm::vec2{0.66f, -0.33f},
    glm::vec2{0.33f, -0.33f},

    glm::vec2{-0.66f, 0.66f},
    glm::vec2{-0.66f, 1.0f},

    glm::vec2{-0.66f, 1.0f},
    glm::vec2{-1.0f, 1.0f},

    glm::vec2{-1.0f, 1.0f},
    glm::vec2{-1.0f, 0.33f},

    glm::vec2{-1.0f, 0.33f},
    glm::vec2{-0.66f, 0.33f},

    glm::vec2{-0.66f, 0.33f},
    glm::vec2{-0.66f, 0.0f},

    glm::vec2{-1.0f, 0.33f},
    glm::vec2{-1.0f, -1.0f},

    glm::vec2{-1.0f, -1.0f},
    glm::vec2{0.0f, -1.0f},

    glm::vec2{0.0f, -1.0f},
    glm::vec2{0.0f, -0.33f},

    glm::vec2{0.0f, -1.0f},
    glm::vec2{0.33f, -1.0f},

    glm::vec2{0.33f, -1.0f},
    glm::vec2{0.33f, -0.66f},

    glm::vec2{0.33f, -0.66f},
    glm::vec2{0.66f, -0.66f},

    glm::vec2{0.33f, -1.0f},
    glm::vec2{1.0f, -1.0f},

    glm::vec2{1.0f, -1.0f},
    glm::vec2{1.0f, 0.0f},

    };

//   std::array const indices{0,1,1,2,2,3};
  // clang-format on

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
//   abcg::glGenBuffers(1, &m_EBO);
//   abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
//   abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
//                      GL_STATIC_DRAW);
//   abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

//   abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

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
//   abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Wall::update(GameData const &gameData, float deltaTime) {
  
  // TODO: Check if hit borders/walls

}