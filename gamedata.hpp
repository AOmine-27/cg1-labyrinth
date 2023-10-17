#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

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

#endif