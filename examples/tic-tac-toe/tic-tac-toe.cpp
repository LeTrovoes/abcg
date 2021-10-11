#include "tic-tac-toe.hpp"

#include <cstdio>
#include <cstdlib>

#include "fmt/core.h"

void TicTacToe::Restart() {
  board.fill(' ');
  turn = 'X';
  winner = 0;
  draw = false;
  playing = true;
}

void TicTacToe::Select(int index) {
  if (index < 0 || index > 8) return;
  if (!playing) return;
  if (board.at(index) != ' ') return;

  board.at(index) = turn;
  turn = turn == 'X' ? 'O' : 'X';
  checkBoard();
}

char TicTacToe::getTurn() const { return turn; }
bool TicTacToe::isPlaying() const { return playing; }
bool TicTacToe::isDraw() const { return draw; }
char TicTacToe::getWinner() const { return winner; }
char TicTacToe::getValueAt(int index) const { return board.at(index); }

void TicTacToe::checkBoard() {
  if (board.at(0) != ' ' && board.at(0) == board.at(1) &&
      board.at(1) == board.at(2)) {
    winner = board.at(0);
    playing = false;
    return;
  }

  if (board.at(3) != ' ' && board.at(3) == board.at(4) &&
      board.at(4) == board.at(5)) {
    winner = board.at(3);
    playing = false;
    return;
  }

  if (board.at(6) != ' ' && board.at(6) == board.at(7) &&
      board.at(7) == board.at(8)) {
    winner = board.at(6);
    playing = false;
    return;
  }

  if (board.at(0) != ' ' && board.at(0) == board.at(3) &&
      board.at(3) == board.at(6)) {
    winner = board.at(0);
    playing = false;
    return;
  }

  if (board.at(1) != ' ' && board.at(1) == board.at(4) &&
      board.at(4) == board.at(7)) {
    winner = board.at(1);
    playing = false;
    return;
  }

  if (board.at(2) != ' ' && board.at(2) == board.at(5) &&
      board.at(5) == board.at(8)) {
    winner = board.at(2);
    playing = false;
    return;
  }

  if (board.at(0) != ' ' && board.at(0) == board.at(4) &&
      board.at(4) == board.at(8)) {
    winner = board.at(0);
    playing = false;
    return;
  }

  if (board.at(2) != ' ' && board.at(2) == board.at(4) &&
      board.at(4) == board.at(6)) {
    winner = board.at(2);
    playing = false;
    return;
  }

  if (board.at(0) != ' ' && board.at(1) != ' ' && board.at(2) != ' ' &&
      board.at(3) != ' ' && board.at(4) != ' ' && board.at(5) != ' ' &&
      board.at(6) != ' ' && board.at(7) != ' ' && board.at(8) != ' ') {
    draw = true;
    playing = false;
    return;
  }
}
