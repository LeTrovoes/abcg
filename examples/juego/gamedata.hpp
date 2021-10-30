#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Fire };
enum class State { Playing, Winner_Red, Winner_Blu, Draw};
enum class PlayerName { Red, Blu };

struct GameData {
  State state{State::Playing};
  std::bitset<3> red_input;  // [fire, left, right]
  std::bitset<3> blu_input;  // [fire, left, right]
};

#endif
