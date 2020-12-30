/**
 * Hex game implementation
 * By RT
 */

#include <iostream>
#include <string>
#include <limits>

#include "lib/hexboard.h"
#include "lib/hexgame.h"
#include "lib/hexai.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

void readint(string title, int &i) {
  cout << title << endl;
  while(!(cin >> i)){
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout << "Invalid integer, please try again: ";
  }
  cin.clear();
  cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main(int argc, char** argv) {
  Game g = Game(11);
  AI ai = AI(g, 10000);

  cout << "Game start - Player B place 'O' to connect North & South sides: " << endl;
  g.printBoard();
  while (!g.isWon()) {
    Player p = g.getPlayer();
    cout << "Player " << p << "'s turn: " << endl;

    if (p == Player::B) {
      int r, c;
      readint("Enter row: ", r);
      readint("Enter col: ", c);

      if (!g.move(r, c)) {
        cout << "Invalid input" << endl;
      } else {
        g.printBoard();
      }
    } else {
      auto [r, c] = ai.simulate(g);
      if (!g.move(r, c)) {
        cout << "AI Program error" << endl;
        return 0;
      } else {
        g.printBoard();
      }
    }
  }

  return 1;
}