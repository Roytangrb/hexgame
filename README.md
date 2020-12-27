# Hexgame

## Setup
* `Bazel version`: bazel 3.7.2-homebrew
* Bazel [setup](https://docs.bazel.build/versions/master/install.html)
* Bazel [C++](https://docs.bazel.build/versions/master/tutorial/cpp.html)

## Commands
* Build: `bazel build //main:hexgame`
* Run: `./bazel-bin/main/hexgame`
* Deps Graph: `bazel query --notool_deps --noimplicit_deps "deps(//main:hexgame)" --output graph`

## Program does
1. Draw the board using ASCII symbols and a given size, such as 7 by 7 or 11 by 11
2. Input a move and determine if a move is legal.
3. Computer opponent move based on Monte Carlo simulation
4. Determine who won.

## Reference
* [Hex Game](https://en.wikipedia.org/wiki/Hex_(board_game))
* [GraphViz](http://www.webgraphviz.com/)
* [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)