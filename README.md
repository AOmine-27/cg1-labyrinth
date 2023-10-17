
# CG1 - Labyrinth

Projeto 1 para a disciplina de Computação Gráfica da UFABC.
Este projeto consiste em um labirinto 2D em que o jogador precisa ir de um lado ao outro via comandos do teclado (AWSD ou as setas)

Em todas as paredes do labirinto é calculado a colisão com o jogador, representado por um cubo, não permitindo então que o labirinto seja atravessado.

Além disso, tanto as paredes quanto o próprio jogador possuem cores definidos pelos shaders, que variam de acordo com a posição do pixel na tela.

## Funcionamento

A estrutura do projeto divide a classe do jogador (`cube.cpp` e `cube.hpp`) da classe do labirinto em si (`wall.cpp` e `wall.hpp`).

### Classe Cube
A classe Cube representa o jogador e possui os métodos `create`, `paint`, `destroy` e `update`.
No `create`, basicamente é obtido dados dos shaders, sua posição inicial, seus vértices e a preparação dos buffers.

Sua construção é feita por definição dos pontos de suas vértices e a ordem em que serão renderizadas em triângulos:

```
m_cubeSidePoints = {
      glm::vec2{0.0f, +0.5f},
      glm::vec2{0.5f, 0.0f},
      glm::vec2{+1.0f, +0.5f},
      glm::vec2{+0.5f, 1.0f},
      };
      
  std::array const indices{ 0, 1, 2, 0, 3, 2};
```

Já sua renderização é realizada utilizando a função `glDrawElements`, passando a primitiva `GL_TRIANGLES`.

Em seu método `update`, é onde ocorre o cálculo do movimento conforme o input dado do teclado. Além disso, também é feito a checagem de colisão e os lados da colisão, que impedem o movimento além das paredes do labirinto:
```
void Cube::update(GameData const &gameData, float deltaTime) {
  auto vel{0.5f};

  if (gameData.m_collision == Collision::True) {
    auto side1 = gameData.m_collisionSide1;
    auto side2 = gameData.m_collisionSide2;
    
    if (gameData.m_input[gsl::narrow<size_t>(Input::Left)] && (side1 != 1 && side2 != 1))
      m_translation -= glm::vec2{0.2f, 0.0f} * deltaTime*vel;
    if (gameData.m_input[gsl::narrow<size_t>(Input::Right)]  && (side1 != 3 && side2 != 3))
      m_translation += glm::vec2{0.2f, 0.0f} * deltaTime*vel;
    if (gameData.m_input[gsl::narrow<size_t>(Input::Up)]  && (side1 != 4 && side2 != 4))
      m_translation += glm::vec2{0.0f, 0.2f} * deltaTime*vel;
    if (gameData.m_input[gsl::narrow<size_t>(Input::Down)]  && (side1 != 2 && side2 != 2))
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
```

---
### Classe Wall
Esta classe representa as paredes do labirinto, possuindo os métodos de sua criação, renderização e deleção.

Sua criação é baseada em um conjunto de 4 coordenadas, cada um indicando uma reta ou "parede" do labirinto no formato (Início em X, Início em Y, Final em X, Final em Y):
```
m_wallArrays = {
    glm::vec4{0.0f, 0.33f, -0.33f, 0.33f},
    glm::vec4{-0.33f, 0.33f, -0.33f, 0.0f},
    glm::vec4{ -0.33f, 0.0f,    0.66f, 0.0f},
    glm::vec4{ 0.66f, 0.0f,    0.66f, 0.33f},
    glm::vec4{ 0.66f, 0.33f,    0.66f, 0.66f},
    glm::vec4{ 0.66f, 0.33f,    1.0f, 0.33f},
    glm::vec4{ 1.0f, 0.33f,    1.0f, 1.0f},
    glm::vec4{ 1.0f, 1.0f,    -0.33f, 1.0f},
    glm::vec4{ -0.33f, 1.0f,    -0.33f, 0.66f},
    glm::vec4{ -0.33f, 0.66f,    0.33f, 0.66f},
    glm::vec4{ 0.33f, 0.66f,    0.33f, 0.33f},
    glm::vec4{ -0.33f, 0.0f,    -0.33f, -0.66f},
    glm::vec4{ -0.33f, -0.66f,    -0.66f, -0.66f},
    glm::vec4{ -0.66f, -0.33f,    -0.66f, -0.66f},
    glm::vec4{ 0.66f, 0.0f,    0.66f, -0.33f},
    glm::vec4{ 0.66f, -0.33f,    0.33f, -0.33f},
    glm::vec4{ -0.66f, 0.66f,    -0.66f, 1.0f},
    glm::vec4{ -0.66f, 1.0f,    -1.0f, 1.0f},
    glm::vec4{ -1.0f, 1.0f,    -1.0f, 0.33f},
    glm::vec4{ -1.0f, 0.33f,    -0.66f, 0.33f},
    glm::vec4{ -0.66f, 0.33f,    -0.66f, 0.0f},
    glm::vec4{ -1.0f, 0.33f,    -1.0f, -1.0f},
    glm::vec4{ -1.0f, -1.0f,    0.0f, -1.0f},
    glm::vec4{ 0.0f, -1.0f,    0.0f, -0.33f},
    glm::vec4{ 0.0f, -1.0f,    0.33f, -1.0f},
    glm::vec4{ 0.33f, -1.0f,    0.33f, -0.66f},
    glm::vec4{ 0.33f, -0.66f,    0.66f, -0.66f},
    glm::vec4{ 0.33f, -1.0f,    1.0f, -1.0f},
    glm::vec4{ 1.0f, -1.0f,    1.0f, 0.0f},
  };
```

Sua renderização é feita utilizando o método `glDrawArrays` passando-se `GL_LINES` como argumento.

Um outro ponto fundamental da implementação é que no `header` foi definido 3 arestas que representam uma `área de vitória`:

```
std::array<glm::vec4, 3> m_winningArea{glm::vec4{1.0f, 0.33f, 1.2f, 0.33f},

                                         glm::vec4{1.2f, 0.33f, 1.2f, 0.0f},

                                         glm::vec4{1.2f, 0.0f, 1.0f, 0.0f}};
```

O jogador colidindo em uma dessas arestas irá alterar o estado do jogo para concluído.

---
### GameData
A estrutura GameData também é utilizada para armazenar dados de estado do jogo como input, estado (`Playing/Finished`), colisão e lados de colisão:

```
enum class Input { Right, Left, Down, Up };
enum class State { Playing, Finished };
enum class Collision { True, False };

struct GameData {
  State m_state{State::Playing};
  Collision m_collision{Collision::False};
  int m_collisionSide1{0};
  int m_collisionSide2{0};
  std::bitset<4> m_input;
};
```

---
### Classe Window
Além dessas duas classes e o GameData, a classe window também é utilizada. As principais partes de implementação nessa classe são a checagem de colisão do jogador com as paredes e a chegagem de vitória:
```
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
```

Inicialmente é zerado a flag de colisão e os lados e então é calculado se alguma vértice do cubo/jogador está dentro de alguma das linhas do labirinto, através da função `isPointInLine`. 

Como o cubo, no cenário criado, pode ter colisão em até dois lados simultaneamente, foi necessário ter duas variáveis de colisão.

Essas duas variáveis e a flag de colisão são armazeadas no GameData, estrutura compartilhada com a classe Cube para poder impedir o movimento caso necessário durante sua função `update`.

```
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
```

Para o cálculo da colisão, é realizado uma comparação entre a distância de um vértice do cubo a cada extremidade de uma parede. Caso elas sejam iguais (ou pelo menos próximas, numa tolerância de diferença de 0.001), é considerado a colisão, caso contrário a vértice do cubo está fora da linha.

Por último, a checagem de vitória utiliza também esse cálculo de colisão, porém utilizando as arestas de vitória do labirinto, que não são renderizadas porém são consideradas somente no cálculo:
```
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
```

## Web
https://aomine-27.github.io/

## Grupo
- 11007015 Daniel Byoung Koo Jung 
- 11062416 André Omine

