#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Fire };
enum class State { Playing, Winner_Red, Winner_Blu, Draw };
enum class PlayerName { Red, Blu };

using PlayerInput = std::bitset<3>;  // [fire, left, right]

struct GameData {
  State state{State::Playing};
  PlayerInput red_input;
  PlayerInput blu_input;
};

#endif
