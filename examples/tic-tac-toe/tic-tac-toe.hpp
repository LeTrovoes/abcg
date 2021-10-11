#include <array>

class TicTacToe {
 public:
  void Restart();
  void Select(int index);
  [[nodiscard]] char getTurn() const;
  [[nodiscard]] bool isPlaying() const;
  [[nodiscard]] bool isDraw() const;
  [[nodiscard]] char getWinner() const;
  [[nodiscard]] char getValueAt(int index) const;

 private:
  std::array<char, 9> board{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
  bool playing = false;
  bool draw = false;
  char turn = ' ';
  char winner = ' ';
  void checkBoard();
};
