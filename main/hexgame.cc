/**
 * Hex game implementation (use g++ -std=c++11 hexgame.cpp)
 * By RT
 * 20th Dec, 2020
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <tuple>
#include <vector>
#include <chrono>
#include <random>
#include <limits>
#include <algorithm>

using namespace std;

enum class Player: short { B, R };
enum class SquareVal: short { Empty, B, R };

// Game board
class Board {
  private:
    vector< vector<SquareVal> > board;

  public:
    // init empty board with size 
    Board(int n = 11): board(n, vector<SquareVal>(n, SquareVal::Empty)) {}

    friend ostream& operator<<(ostream& out, const Board& b);

    SquareVal getSquare(int r, int c) const {
      return board[r][c];
    }

    void setSquare(int i, int j, SquareVal val) {
      board[i][j] = val;
    }

    int size() const {
      return board.size();
    }

    bool isWithinBoard(int r, int c) const {
      return r >= 0 && r < board.size()
          && c >= 0 && c < board.size();
    }

    bool isAdjacent(int r1, int c1, int r2, int c2) const {
      // if both sqaures are within board and not the same sqaure
      if (isWithinBoard(r1, c1) && isWithinBoard(r2, c2) && !(r1 == r2 && c1 == c2)) {
        // if the euclidean distance is 1 or sqrt(2)
        int dist_pow = (r1 - r2) * (r1 - r2) + (c1 - c2) * (c1 - c2);
        if (dist_pow == 1) return true;
        if (dist_pow == 2 && ((r1 - r2) + (c1 - c2)) == 0) return true;
      }

      return false;
    }

    vector< tuple<int, int> > getNeighbors(int r, int c, SquareVal val) const {
      tuple<int, int> positions[] = {
        make_tuple(r, c-1),
        make_tuple(r, c+1),
        make_tuple(r-1, c),
        make_tuple(r+1, c),
        make_tuple(r+1, c-1),
        make_tuple(r-1, c+1),
      };

      vector< tuple<int, int> > neighbors;
      for (auto pos: positions) {
        auto [r, c] = pos; // C++17 structured binding
        if (isWithinBoard(r, c) && getSquare(r, c) == val) {
          neighbors.push_back(pos);
        }
      }

      return neighbors;
    }

    int copy(const Board &b) {
      int count = 0;
      for (int r = 0; r < b.size(); r ++) {
        for (int c = 0; c < b.size(); c ++) {
          setSquare(r, c, b.getSquare(r, c));
          if (getSquare(r, c) != SquareVal::Empty) count++;
        }
      }
      return count;
    }

    ~Board(){}
};

// Game controller
class Game {
  private:
    Player player; // which player's turn
    Board board;
    bool won;

  public:
    // new game starts with player B's turn
    Game(int n = 11): board(n), player(Player::B), won(false) {}

    friend class AI;

    // return `true` if move is valid
    bool move(int r, int c) {
      if (validate(r, c)) {
        SquareVal val = player == Player::B ? SquareVal::B : SquareVal::R;
        board.setSquare(r, c, val);
        checkWon(r, c, val);

        // switch player
        player = player == Player::B ? Player::R : Player::B;

        return true;
      }

      return false;
    }

    bool validate(int r, int c) const {
      return board.isWithinBoard(r, c)
          && board.getSquare(r, c) == SquareVal::Empty;
    }

    // if game is won, the curr move is the last move
    // check the connected graph of last move has a path
    void checkWon(int r, int c, SquareVal val) {
      int n = board.size();
      // prevent recursion loop, visit once
      vector< vector<bool> >visited(n, vector<bool>(n, false));
      // memo if four boundary is reached
      vector<bool>memo(4, false); // 0: N, 1: S, 2: W, 3: E

      traverseNeighbors(r, c, val, visited, memo);

      if (memo[0] && memo[1] && val == SquareVal::B) {
        cout << "Player B has won" << endl;
        won = true;
      } else if (memo[2] && memo[3] && val == SquareVal::R) {
        cout << "Player R has won" << endl;
        won = true;
      }
    }

    void traverseNeighbors(int r, int c, SquareVal val, vector< vector<bool> > &visited, vector<bool> &memo) {
      visited[r][c] = true;
      if (r == 0) memo[0] = true;
      if (r == board.size()-1) memo[1] = true;
      if (c == 0) memo[2] = true;
      if (c == board.size()-1) memo[3] = true;

      vector< tuple<int, int> > neighbors = board.getNeighbors(r, c, val);
      for (auto neighbor: neighbors){
        auto [nr, nc] = neighbor;
        if (!visited[nr][nc]) {
          traverseNeighbors(nr, nc, val, visited, memo);
        }
      }
    }

    bool isWon() const {
      return won;
    }

    void printBoard() {
      cout << board;
    }

    Player getPlayer() const {
      return player;
    }

    ~Game(){}
};

// Game AI Opponent simulate move based on Game state
// friend of Game
class AI {
  private:
    int n; // n of monte carlo test of each move simulate
    Board sim_board;
  public:
    AI(const Game& g, int n = 1000):
      sim_board(g.board.size()),
      n(n) {}

    tuple<int, int> simulate(Game &g) {
      unsigned seed = chrono::system_clock::now().time_since_epoch().count();
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

    bool checkRWon(int r, int c, const Board &board) {
      int n = board.size();
      vector< vector<bool> >visited(n, vector<bool>(n, false));

      return traverseNeighbors(board, r, c, visited);
    }

    bool traverseNeighbors(const Board &board, int r, int c, vector< vector<bool> > &visited) {
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

    ~AI(){}
};

// print board
ostream& operator<<(ostream& out, const Board& b) {
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

ostream& operator<<(ostream& out, const Player& p) {
  char label = p == Player::B ? 'B' : 'R';
  out << label;

  return out;
}

void readint(string title, int &i) {
  cout << title << endl;
  while(!(cin >> i)){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid integer, please try again: ";
  }
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main(void) {
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