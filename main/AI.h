/**
 * Opponent makes move based on Monte Carlo simulation
 * By RT
 */

#ifndef HEX_AI_H_
#define HEX_AI_H_

#include <vector>
#include <tuple>

#include "Board.h"
#include "Game.h"

using std::vector;
using std::tuple;

/**
 * @property {int} n number of monte carlo simulation before each move
 * @property {Board} sim_board board for simulation
 */
class AI {
  private:
    int n;
    Board sim_board;
  public:
    AI(const Game& g, int n = 1000);

    /**
     * @return {tuple<int, int>} sugguested move of (r, c)
     */
    tuple<int, int> simulate(Game &g);

    bool checkRWon(int r, int c, const Board &board) const;

    bool traverseNeighbors(
      const Board &board,
      int r,
      int c,
      vector< vector<bool> > &visited
    ) const;

    ~AI(){}
};

#endif
