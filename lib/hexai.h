/**
 * Opponent makes move based on Monte Carlo simulation
 * By RT
 */

#ifndef HEX_AI_H_
#define HEX_AI_H_

#include <vector>
#include <tuple>

#include "lib/hexboard.h"
#include "lib/hexgame.h"

namespace hexai {

/**
 * @property {int} n number of monte carlo simulation before each move
 * @property {Board} sim_board board for simulation
 */
class AI {
  private:
    int n;
    hexboard::Board sim_board;
  public:
    AI(const hexgame::Game& g, int n = 1000);

    /**
     * @return {tuple<int, int>} sugguested move of (r, c)
     */
    std::tuple<int, int> simulate(hexgame::Game &g);

    bool checkRWon(int r, int c, const hexboard::Board &board) const;

    bool traverseNeighbors(
      const hexboard::Board &board,
      int r,
      int c,
      std::vector< std::vector<bool> > &visited
    ) const;

    ~AI(){}
};

} // namespace hexai

#endif

