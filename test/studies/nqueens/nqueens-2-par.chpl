// A parallel backtracking solver for the N Queens problem.
// This is a parallelization of the sequential solver in queens-seq.chpl.

// how big the board to play
config var N = 8;

// up to how big a board to print solution counts for
config var N_counts_only = 9;

// parallelization: recurse in parallel when this many rows have been filled
// (larger parRow makes it create more tasks)
// (it is 'const' so we can correctly deallocate boards in tryQueenInNextRow())
config const parRow: int = 3;

// ancillary: count the tasks; to be used from the initial thread only
var taskCount: int = 0;

// ancillary: count the solutions
var solutionCount$: sync int;

// ancillary: show each solution as it is discovered?
config var showEachSolution: bool = true;

// The main program.
//
proc main() {
  if N > 0 then
    countSolutions(N, showEachSolution);
  for n in 1..N_counts_only do
    countSolutions(n, false);
}

// Find all solutions for the given board size, optionally showing each,
// and print their count.
//
proc countSolutions(boardSize: int, showEachSoln: bool) {
  solutionCount$ = 0;
  showEachSolution = showEachSoln;
  if showEachSoln then
    writeln("Solving N Queens for N=", boardSize,
            " in parallel from level ", parRow, "...");
  sync {
    var board = createBoard(boardSize);

    tryQueenInNextRow(board);  // forego dealloc of this board

    delete board;
  }
  writeln("Found ", solutionCount$.readFE(), " solutions for N=", boardSize);
}

/////////////////////////////////////////////////////////////////////////////
// the algorithm

// Given a partially-filled board, we try placing a queen
// in the next row - trying each column in turn.
// If the column succeeds, we proceed to the next row
// (or show the result if we have filled all rows).
//
proc tryQueenInNextRow(board: unmanaged Board): void {
  // iterate over the columns
  for col in 1..board.boardSize {
    // place the queen in that column if legal
    if board.placeNextIfLegal(col) {
      if board.lastfilled == board.boardSize {
        // found a complete solution
        solutionCount$ += 1;
        if showEachSolution then writeln(board);
      } else if board.lastfilled == parRow {
        taskCount += 1;
	var clone = board.clone(taskCount);
        // recurse in parallel
	begin tryQueenInNextRow(clone);
      } else {
        // recurse within the same task
        tryQueenInNextRow(board);
      }
      // remove the placed queen so we can try another column
      // (this is merely to satisfy our assertions)
      board.removeLast(col);
    }
  }  // for

  // deallocate the clone board if we are done with it
  if board.lastfilled == parRow then
    delete board;
}

/////////////////////////////////////////////////////////////////////////////
// the data structure
//
// Maintains the placement of the queens.
// Only chess-legal placements are stored.
//
// The following *in-place* updates are supported:
// - place a new queen
//   (queens must be placed in consecutive rows, starting at row 1), and
// - remove a queen
//   (queens must be removed in the LIFO order).
//
// Record-style value copying can be done with Board.clone().
//
class Board {

  // size of the board
  const boardSize: int;

  // queencol(r) is the column the queen is at in row r, 0 if none
  var queencol: [1..boardSize] int;

  // the largest row where a queen has been placed, 0 if none
  var lastfilled: int = 0;

  // ancillary: the task number: make output deterministic by sorting on it
  var taskNum: int;

} // class Board

// Create an empty board of the given size.
// NB could not do this by writing our own constructor.
//
proc createBoard(boardSize:int) {
  return new unmanaged Board(boardSize = boardSize);
}

// Return a (newly-created) clone of this board.
//
proc Board.clone(taskNumArg: int): unmanaged Board {

  // Linguistic remark: this code looks funny, but it does the following.
  // It invokes Board's default constructor (since we have not defined any
  // ourselves) using by-name parameter passing. The names to the
  // left of the equal signs are the constructor's argument names
  // (which are the names of Board's fields, by definition).
  // The names to the right of the equals are the fields of the 'this'
  // object, which is also a Board. The exception is taskNum of the
  // newly-created object, which we want to initialize to the argument
  // rather than to this.taskNum. So we give the argument a (slightly)
  // different name to make our intentions (slightly) clear.
  //
  return new unmanaged Board(boardSize  = boardSize,
                             queencol   = queencol,
                             lastfilled = lastfilled,
                             taskNum    = taskNumArg);
}

// If placing a queen at (lastfilled+1,col) is legal, do so and return true.
// Otherwise, don't and return false.
//
proc Board.placeNextIfLegal(col: int): bool {
  if nextPlacementIsLegal(col) {
    lastfilled += 1;
    queencol(lastfilled) = col;
    return true;
  } else {
    return false;
  }
}

// Remove the most recently placed queen (that hasn't been removed yet).
//
proc Board.removeLast(col: int): void {
  lastfilled -= 1;
}

// Helper: would placing a new queen at (lastfilled+1,col) be legal?
// Assume the existing placement is legal.
//
proc Board.nextPlacementIsLegal(col: int): bool {
  var row = lastfilled + 1;
  for prevRow in 1..lastfilled {
    var prevCol = queencol(prevRow);
    if
      prevCol == col ||
      // check the diagonals
      prevCol - col == prevRow - row ||
      prevCol - col == row - prevRow
    then
      return false;
  }

  // everything is hunky-dory
  return true;
}

// Display the board.
//
config var show1line: bool = true;

override proc Board.writeThis(f) throws {
  if boardSize <= 0 {
    f.write( taskNum, ": the board is empty");
    return;
  }
  var notFilledMsg = "";
  if lastfilled < boardSize then notFilledMsg =
    " row(s) "+ (lastfilled + 1):string + " to " + boardSize:string + " are not filled";
  if show1line {
    f.write(
            taskNum, ": ",
            [row in 1..lastfilled] (row, queencol(row)),
            notFilledMsg);
  } else {
    for {1..boardSize} do f.write("-"); f.writeln();
    for row in 1..lastfilled {
      for col in 1..boardSize do
        f.write(if queencol(row) == col then "*" else " ");
      f.writeln();
    }
    if notFilledMsg != "" then f.writeln(notFilledMsg);
    for {1..boardSize} do f.write("-");
  }
}
