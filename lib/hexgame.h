/**
 * Hex game state and rule controller
 * By RT
 */

#ifndef HEX_GAME_H_
#define HEX_GAME_H_

#include <iostream>
#include <vector>

#include "lib/hexboard.h"

using std::ostream;
using std::vector;

enum class Player: short { B, R };

ostream& operator<<(ostream& out, const Player& p);

/**
 * @property {Player} player current to make move
 * @property {Board} board
 * @property {bool} won is game won
 */
class Game {
  private:
    Player player;
    Board board;
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

    void checkWon(int r, int c, SquareVal val);

    /**
     * traverse the board recursively starting at(r, c)
     * memo if four borders are reached
     */
    void traverseNeighbors(
      int r,
      int c,
      SquareVal val,
      vector< vector<bool> > &visited,
      vector<bool> &memo
    ) const;

    bool isWon() const;

    void printBoard() const;

    Player getPlayer() const;

    friend class AI;

    ~Game(){}
};

#endif