use IO;

config const inputfile = "easy.dat";


// Attempt to solve the given Sudoku board.  Note: the simple solver
// isn't able to completely solve really hard puzzles.
proc solveBoard(inout board: [] int) {
  var elimBoard: [1..9, 1..9, 1..9] bool = true;
  var nUpdates = 0;

  // For each spot that's filled in, mark it as unavailable in the list for
  // each other spot in its row, column, and block
  for (i, j) in board.domain do
    if (board[i, j] > 0) {
      elimBoard[i, 1..9, board[i, j]] = false;
      elimBoard[1..9, j, board[i, j]] = false;
      elimBoard[(i-1)/3*3+1..(i-1)/3*3+3, (j-1)/3*3+1..(j-1)/3*3+3, board[i, j]] = false;
      //writeln("(", i, ", ", j, "): irange = [", (i-1)/3*3+1, "..", (i-1)/3*3+3, "]  jrange = [",
      //  (j-1)/3*3+1, "..", (j-1)/3*3+3, "]");
    }

  do {
    nUpdates = 0;

    // Find spaces where there's only one possible entry and fill them in
    for (i, j) in board.domain {
      const NONE = 0, MULTIPLE = -1;
      var   idx  = NONE;

      if board[i, j] > 0 then continue;

      for k in 1..9 do
        if elimBoard[i, j, k] then
          idx = if idx == NONE then k else MULTIPLE;

      if idx > 0 {
        board[i, j] = idx;
        elimBoard[i, 1..9, board[i, j]] = false;
        elimBoard[1..9, j, board[i, j]] = false;
        elimBoard[(i-1)/3*3+1..(i-1)/3*3+3, (j-1)/3*3+1..(j-1)/3*3+3, board[i, j]] = false;
        nUpdates   += 1;
      }
    }

    writeln(" - Udates this round: ", nUpdates);
  } while (nUpdates > 0);
}


proc main() {
  var board: [1..9, 1..9] int;
  var boardData = open(inputfile, iomode.r).reader();

  // Read the board from the input file

  writeln("Reading board from: ", inputfile, "\n");
  for element in board do boardData.read(element);
  
  boardData.close();
  writeln(board, "\n");

  solveBoard(board);

  // Count up the remaining blanks
  var nSpots = 0;
  [num in board with (ref nSpots)] if num == 0 then nSpots += 1; // race

  if nSpots == 0 then 
    writeln("\nPuzzle solved:\n\n", board);
  else 
    writeln("\nI was able to fill in all but ", nSpots, 
        " spaces, good luck with the rest!\n\n", board);
}
