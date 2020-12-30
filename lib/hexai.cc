/**
 * Opponent makes move based on Monte Carlo simulation
 * By RT
 */

#include <tuple>
#include <chrono>
#include <random>
#include <algorithm>

#include "lib/hexboard.h"
#include "lib/hexgame.h"
#include "lib/hexai.h"

using std::tuple;
using std::make_tuple;
using std::default_random_engine;
using std::shuffle;

AI::AI(const Game& g, int n):
  sim_board(g.board.size()),
  n(n) {}

tuple<int, int> AI::simulate(Game &g) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  default_random_engine gen(seed);

  int board_size = sim_board.size();
  int n_squares = board_size * board_size;
  vector<SquareVal> moves(n_squares, SquareVal::Empty);
  
  for (int i = 0; i < moves.size(); i++) {
    moves[i] = i % 2 == 0 ? SquareVal::B : SquareVal::R; // player B move first
  }

  // monte carlo fill board randomly and determine wins
  // increment score of a position if R won in the simulated board
  vector< vector<int> >scores(board_size, vector<int>(board_size, 0));
  for (int i = 0; i < this->n; i ++) {
    // copy existing moves to simulate board
    int n_exists_moves = sim_board.copy(g.board);

    // skip existing move count
    shuffle(moves.begin() + n_exists_moves, moves.end(), gen);

    int i_rand_move = n_exists_moves;
    for (int i = 0; i < moves.size(); i++) {
      int r = i/board_size, c = i%board_size;
      if (sim_board.getSquare(r, c) == SquareVal::Empty){
        sim_board.setSquare(r, c, moves[i_rand_move++]);
      }
    }
    
    // check won of a completely filled board
    bool r_won = false;
    for (int r = 0; r < board_size; r++){
      if (sim_board.getSquare(r, 0) == SquareVal::R){
        r_won = checkRWon(r, 0, sim_board);
        if (r_won) break;
      }
    }

    if (r_won) {
      for (int r = 0; r < board_size; r++){
        for (int c = 0; c < board_size; c++){
          if (sim_board.getSquare(r, c) == SquareVal::R){
            scores[r][c]++;
          }
        }
      }
    }
  }

  // after n simulation, suggest a valid highest scored move
  int max_r = 0, max_c = 0, max_score = 0;
  for (int r = 0; r < board_size; r++){
    for (int c = 0; c < board_size; c++){
      // exclude already occupied squares
      if (g.board.getSquare(r, c) != SquareVal::Empty){
        scores[r][c] = -1;
      }
      //memo position of max score move
      if (scores[r][c] >= max_score) {
        max_score = scores[r][c];
        max_r = r;
        max_c = c;
      }
    }
  }

  return make_tuple(max_r, max_c);
}

bool AI::checkRWon(int r, int c, const Board &board) const {
  int n = board.size();
  vector< vector<bool> >visited(n, vector<bool>(n, false));

  return traverseNeighbors(board, r, c, visited);
}

bool AI::traverseNeighbors(
  const Board &board,
  int r,
  int c,
  vector< vector<bool> > &visited
  ) const
{
  visited[r][c] = true;
  if (c == board.size()-1) return true;

  vector< tuple<int, int> > neighbors = board.getNeighbors(r, c, SquareVal::R);
  for (auto neighbor: neighbors){
    auto [nr, nc] = neighbor;
    if (!visited[nr][nc]) {
      return false || traverseNeighbors(board, nr, nc, visited);
    }
  }
  return false;
}
