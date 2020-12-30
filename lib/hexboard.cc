/**
 * 2D matrix board to store value of each sqaure
 * By RT
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <tuple>

#include "lib/hexboard.h"

namespace hexboard {

Board::Board(int n):
  board(n, std::vector<SquareVal>(n, SquareVal::Empty)) {}

SquareVal Board::getSquare(int r, int c) const {
  return board[r][c];
}

void Board::setSquare(int r, int c, SquareVal val) {
  board[r][c] = val;
}

int Board::size() const {
  return board.size();
}

bool Board::isWithinBoard(int r, int c) const {
  return r >= 0 && r < board.size()
      && c >= 0 && c < board.size();
}

bool Board::isAdjacent(int r1, int c1, int r2, int c2) const {
  // if both sqaures are within board and not the same sqaure
  if (isWithinBoard(r1, c1) && isWithinBoard(r2, c2) && !(r1 == r2 && c1 == c2)) {
    // if the euclidean distance is 1 or sqrt(2)
    int dist_pow = (r1 - r2) * (r1 - r2) + (c1 - c2) * (c1 - c2);
    if (dist_pow == 1) return true;
    if (dist_pow == 2 && ((r1 - r2) + (c1 - c2)) == 0) return true;
  }

  return false;
}

std::vector< std::tuple<int, int> > Board::getNeighbors(int r, int c, SquareVal val) const {
  std::tuple<int, int> positions[] = {
    std::make_tuple(r, c-1),
    std::make_tuple(r, c+1),
    std::make_tuple(r-1, c),
    std::make_tuple(r+1, c),
    std::make_tuple(r+1, c-1),
    std::make_tuple(r-1, c+1),
  };

  std::vector< std::tuple<int, int> > neighbors;
  for (auto pos: positions) {
    auto [r, c] = pos; // C++17 structured binding
    if (isWithinBoard(r, c) && getSquare(r, c) == val) {
      neighbors.push_back(pos);
    }
  }

  return neighbors;
}

int Board::copy(const Board &b) {
  int count = 0;
  for (int r = 0; r < b.size(); r ++) {
    for (int c = 0; c < b.size(); c ++) {
      setSquare(r, c, b.getSquare(r, c));
      if (getSquare(r, c) != SquareVal::Empty) count++;
    }
  }
  return count;
}

std::ostream& operator<<(std::ostream& out, const Board& b) {
  using std::string;
  using std::endl;
  using std::setw;
  using std::setfill;
  using std::setiosflags;
  using std::ios_base;

  int row_indent = 1;
  // print header row
  out << setw(4) << setfill(' ') << ' ';
  for (int i = 0; i < b.size(); i ++){
    out << i << "   ";
  }
  out << endl;
  for (auto const &row: b.board) {
    // set row number
    out << setw(2) << setfill(' ') << setiosflags(ios_base::left) << row_indent-1;

    out << setw(row_indent * 2) << setfill(' ') << ' ';
    int col = 0;
    for (auto const &val: row) {
      char label = val == SquareVal::Empty ? '.' : (val == SquareVal::R ? 'X' : 'O');
      string delimeter = ++col == b.size() ? "" : " - ";
      out << label << delimeter;
    }
    out << endl;
    // print links to next row
    if (row_indent < b.size()){
      out << setw(row_indent * 2 + 2) << setfill(' ') << ' ';
      out << " \\";
      for (int i = 1; i < b.size(); i ++){
        out << " / \\";
      }
      out << endl;

      row_indent++;
    }
  }

  return out;
}

} // namespace hexboard