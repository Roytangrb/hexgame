/**
 * Hex game state and rule controller
 * By RT
 */

#include <iostream>
#include <vector>
#include <tuple>

#include "lib/hexboard.h"
#include "lib/hexgame.h"

namespace hexgame {

// new game starts with player B's turn
Game::Game(int n):
  board(n), player(Player::B), won(false) {}

bool Game::move(int r, int c) {
  if (validate(r, c)) {
    hexboard::SquareVal val = 
      player == Player::B ? 
        hexboard::SquareVal::B :
        hexboard::SquareVal::R ;

    board.setSquare(r, c, val);
    checkWon(r, c, val);

    // switch player
    player = player == Player::B ? Player::R : Player::B;

    return true;
  }

  return false;
}

bool Game::validate(int r, int c) const {
  return board.isWithinBoard(r, c)
      && board.getSquare(r, c) == hexboard::SquareVal::Empty;
}

/**
 * if game is won, the curr move is the last move
 * check the connected graph of last move has a path
 */
void Game::checkWon(int r, int c, hexboard::SquareVal val) {
  int n = board.size();
  // prevent recursion loop, visit once
  std::vector< std::vector<bool> >visited(n, std::vector<bool>(n, false));
  // memo if four boundary is reached
  std::vector<bool>memo(4, false); // 0: N, 1: S, 2: W, 3: E

  traverseNeighbors(r, c, val, visited, memo);

  if (memo[0] && memo[1] && val == hexboard::SquareVal::B) {
    std::cout << "Player B has won" << std::endl;
    won = true;
  } else if (memo[2] && memo[3] && val == hexboard::SquareVal::R) {
    std::cout << "Player R has won" << std::endl;
    won = true;
  }
}

void Game::traverseNeighbors(
  int r,
  int c,
  hexboard::SquareVal val,
  std::vector< std::vector<bool> > &visited,
  std::vector<bool> &memo) const
{
  visited[r][c] = true;
  if (r == 0) memo[0] = true;
  if (r == board.size()-1) memo[1] = true;
  if (c == 0) memo[2] = true;
  if (c == board.size()-1) memo[3] = true;

  std::vector< std::tuple<int, int> > neighbors = board.getNeighbors(r, c, val);
  for (auto neighbor: neighbors){
    auto [nr, nc] = neighbor;
    if (!visited[nr][nc]) {
      traverseNeighbors(nr, nc, val, visited, memo);
    }
  }
}

bool Game::isWon() const {
  return won;
}

void Game::printBoard() const {
  std::cout << board;
}

Player Game::getPlayer() const {
  return player;
}

std::ostream& operator<<(std::ostream& out, const Player& p) {
  char label = p == Player::B ? 'B' : 'R';
  out << label;

  return out;
}

} // namespace hexgame