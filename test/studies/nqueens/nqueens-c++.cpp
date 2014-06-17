// This is a C+ version of the Chapel sequential N Queens solver
// (see nqueens-seq.chpl).
// 
// For expediency, this version is simplified by omitting
// some of the detailed output of the Chapel version, e.g.
// detailed halt() and debug() messages.
//
// Note that this gives the C++ version a slight performance advantage
// because debug() (present in Chapel but elided in C++) executes
// a compare and a branch.
//
// This version is also omitting most of the comments explaining
// what is going on. These comments can be found in the Chapel version.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/////////////////////////////////////////////////////////////////////////////
// Our configuration variables

// show the board all on a single line?
bool show1line = true;

// how big the board to play
int N = 8;

/////////////////////////////////////////////////////////////////////////////
// Our datatype

class Board {
public:  

  // size of the board
  const int boardSize;

  // queenvecArr[row-1] is the column the queen is at, 0 if none
  int* const queenvecArr;

  // make indexing look the same as in the Chapel version
# define queenvec(ix) queenvecArr[(ix)-1]

  // the largest row that has been filled in, 0 if none
  int lastfilled;

  // constructor: create an empty board of the given size
  // (note difference from createBoard() in the Chapel version)
  Board(int boardSize);

  // destructor
  ~Board();

  // If placing a queen at (row,col) is legal, do so and return true.
  // Otherwise, don't and return false.
  bool placeNextIfLegal(int row, int col);

  // Remove a queen in the LIFO order.
  void removeLast(int row, int col);

  // Display the board.
  void show();

private:
  // Would the proposed placement be legal?
  // Assume the existing placement is legal.
  bool nextPlacementIsLegal(int row, int col);

};  // class Board

Board::Board(int _boardSize) :
  boardSize(_boardSize),
  queenvecArr(new int[boardSize]),
  lastfilled(0)
{ }

Board::~Board() {
  // in Chapel, owned arrays are deallocated automatically
  delete[] queenvecArr;
}

bool Board::placeNextIfLegal(int row, int col) {
  assert(lastfilled < boardSize);
  assert(1 <= col && col <= boardSize);
  assert(row == lastfilled + 1);
  assert(queenvec(row) == 0);

  if (nextPlacementIsLegal(row, col)) {
    queenvec(row) = col;
    lastfilled += 1;
    return true;
  } else {
    return false;
  }
}

void Board::removeLast(int row, int col) {
  assert(row == lastfilled);
  assert(queenvec(row) == col);

  queenvec(row) = 0;
  lastfilled -= 1;
}


bool Board::nextPlacementIsLegal(int row, int col) {
  for (int i = 1; i <= lastfilled; i++) {
    if (
      queenvec(i) == col ||
      queenvec(i) - col == i - row ||
      queenvec(i) - col == row - i
    ) {
      return false;
    }
  }

  // everything is hunky-dory
  return true;
}

void Board::show() {
  if (boardSize <= 0) {
    printf("the board is empty\n");
    return;
  }
  char notFilledMsg[64];
  if (lastfilled < boardSize) {
    sprintf(notFilledMsg, " row(s) %d to %d are not filled\n",
            (lastfilled + 1), boardSize);
  } else {
    sprintf(notFilledMsg, "");
  }
  if (show1line) {
    for (int row = 1; row <= lastfilled; row++) {
      printf("%s(%d, %d)", (row > 1 ? " " : ""), row, queenvec(row));
    }
    printf("%s\n", notFilledMsg);
  } else {
    for (int i = 1; i <= boardSize; i++) printf("-"); printf("\n");
    for (int row = 1; row <= lastfilled; row++) {
      for (int col = 1; col <= boardSize; col++) {
        printf("%c", queenvec(row) == col ? "*" : " ");
      }
      printf("\n");
    }
    if (notFilledMsg[0] != '\0') printf("%s\n", notFilledMsg);
    for (int i = 1; i <= boardSize; i++) printf("-"); printf("\n");
  }
}

/////////////////////////////////////////////////////////////////////////////
// the algorithm

// ancillary: count the solutions
int solutionCount;

void tryQueenInNextRow(Board* board) {
  // the row we will be placing in
  int nextRow = board->lastfilled + 1;

  // iterate over the columns
  for (int col = 1; col <= board->boardSize; col++) {
    // place the queen in that column if legal
    if (board->placeNextIfLegal(nextRow, col)) {
      if (nextRow == board->boardSize) {
        // found a complete solution
        solutionCount++;
        board->show();
      } else {
        tryQueenInNextRow(board);
      }
      // remove the placed queen so we can try another column
      // (this is merely to satisfy our assertions)
      board->removeLast(nextRow, col);
    }
  }
}  // tryQueenInNextRow

/////////////////////////////////////////////////////////////////////////////
// the driver

// just for kicks
void helpAndQuit(int rc) {
  printf("Usage:  queens [OPTIONS]\n"
         "OPTIONS:\n"
         "  --N=num           solve N Queens for a board of the size 'num'\n"
         "  --show1line=true  print boards in a single-line format\n"
         "  --show1line=false print boards nicely on multiple lines\n"
         );
  exit(rc);
}

// The following is built into the Chapel language,
// but we have to do it by hand in C++.
void processArguments(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    char* arg = argv[i];
    if (!strncmp(arg, "--N=", 4)) {
      N = atoi(arg+4);
      if (N <= 0) helpAndQuit(2);
    } else if (!strcmp(arg, "--show1line=true")) {
      show1line = true;
    } else if (!strcmp(arg, "--show1line=false")) {
      show1line = false;
    } else if (!strcmp(arg, "-h") || !strcmp(arg, "--help")) {
      helpAndQuit(0);
    } else {
      helpAndQuit(2);
    }
  }
}

// The main program.
// 
//
int main(int argc, char** argv) {
  processArguments(argc, argv);
  solutionCount = 0;
  printf("Solving N Queens for N=%d...\n", N);
  tryQueenInNextRow(new Board(N));   // elide dealloc of this board
  printf("Found %d solutions for N=%d\n", solutionCount, N);
  return 0;
}
