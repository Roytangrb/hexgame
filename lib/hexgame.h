/**
 * Hex game state and rule controller
 * By RT
 */

#ifndef HEX_GAME_H_
#define HEX_GAME_H_

#include <iostream>
#include <vector>

#include "lib/hexboard.h"

// forward delaration of namespace friend
namespace hexai {
  class AI;
}

namespace hexgame {

enum class Player: short { B, R };

std::ostream& operator<<(std::ostream& out, const Player& p);

/**
 * @property {Player} player current to make move
 * @property {Board} board
 * @property {bool} won is game won
 */
class Game {
  private:
    Player player;
    hexboard::Board board;
    bool won;

  public:
    /**
     * init new game with board size
     * new game starts with player B's turn
     * @param {int} n size of board
     */
    Game(int n = 11);

    /**
     * return {bool} true if move is valid
     */
    bool move(int r, int c);

    bool validate(int r, int c) const;

    void checkWon(int r, int c, hexboard::SquareVal val);

    /**
     * traverse the board recursively starting at(r, c)
     * memo if four borders are reached
     */
    void traverseNeighbors(
      int r,
      int c,
      hexboard::SquareVal val,
      std::vector< std::vector<bool> > &visited,
      std::vector<bool> &memo
    ) const;

    bool isWon() const;

    void printBoard() const;

    Player getPlayer() const;

    friend class hexai::AI;

    ~Game(){}
};

} // namespace hexgeame

#endif