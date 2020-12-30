/**
 * 2D matrix board to store value of each sqaure
 * By RT
 */

#ifndef HEX_BOARD_H_
#define HEX_BOARD_H_

#include <iostream>
#include <vector>
#include <tuple>

namespace hexboard {

enum class SquareVal: short { Empty, B, R };

class Board {
  private:
    std::vector< std::vector<SquareVal> > board;

  public:
    /**
     * init empty board with size 
     * @param {int} n size of board
     */
    Board(int n = 11);

    SquareVal getSquare(int r, int c) const;

    void setSquare(int r, int c, SquareVal val);

    int size() const;

    /**
     * validate position (r, c) of sized board
     */
    bool isWithinBoard(int r, int c) const;
    
    /**
     * check if (r1, c1) and (r2, c2) are adjacent sqaures
     */
    bool isAdjacent(int r1, int c1, int r2, int c2) const;

    /**
     * get sqaure (r, c)'s neighbors in tuple(r, c) vector
     */
    std::vector< std::tuple<int, int> > getNeighbors(int r, int c, SquareVal val) const;

    /**
     * copy squares values from another board
     * @return {int} number of sqaures that are not empty
     */
    int copy(const Board &b);

    friend std::ostream& operator<<(std::ostream& out, const Board& b);

    ~Board(){}
};

} // namespace hexboard

#endif