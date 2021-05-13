// A parallel backtracking solver for the N Queens problem.
// This is a parallelization of the sequential solver in queens-seq.chpl.

/////////////////////////////////////////////////////////////////////////////
// the data structure
//
// Maintains the placement of the queens.
// Only chess-legal placements are stored.
//
// Provides efficient updates in-place (we expect this to be useful).
// Record-style value copying can be done with Board.clone().
//
// Queens must be placed in consecutive rows, starting at row 1.
// Queens must be removed in the LIFO order.
//
class Board {

  // size of the board
  const boardSize: int;

  // queenvec(row) is the column the queen is at, 0 if none
  var queenvec: [1..boardSize] int;

  // the largest row that has been filled in, 0 if none
  var lastfilled: int = 0;

  // ancillary: the task number: make output deterministic by sorting on it
  var taskNum: int;

}  // class Board

//
// Public interface.
// Create an empty board of the given size.
// NB could not do this by writing our own constructor.
//
proc createBoard(boardSize:int) {
  return new unmanaged Board(boardSize = boardSize);
}

//
// Public interface.
// If placing a queen at (row,col) is legal, do so and return true.
// Otherwise, don't and return false.
//
proc Board.placeNextIfLegal(row: int, col: int): bool {
  debug("adding   ", row, ", ", col);
  if !(lastfilled < boardSize) then
    halt("attempting to add a queen past the board's capacity");
  if !(1 <= col && col <= boardSize) then
    halt("illegal column: ", col, " attempted to be added at row ", row);
  if !(row == lastfilled + 1) then
    halt("the next queen must be placed in row ", lastfilled + 1,
         ", attempted to place it in row ", row);

  if !(queenvec(row) == 0) then
    halt("there is already a queen at row ", row,
	 " (column ", queenvec(row), ")");

  if nextPlacementIsLegal(row, col) then {
    queenvec(row) = col;
    lastfilled += 1;
    return true;
  } else {
    return false;
  }
}

//
// Public interface.
// Remove a queen in the LIFO order.
//
proc Board.removeLast(row: int, col: int): void {
  debug("removing ", row, ", ", col, "\n");
  if !(row == lastfilled) then
    halt("attempted to remove a queen in a non-LIFO order",
         " from row ", row, ", should be row ", lastfilled);
  if !(queenvec(row) == col) then
    halt("attempted to remove a queen at row ", row,
         " from column ", col, ", should be column ", queenvec(row));

  queenvec(row) = 0;  // this is merely to satisfy our assertions
  lastfilled -= 1;
}

//
// Public interface.
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
                             queenvec   = queenvec,
                             lastfilled = lastfilled,
                             taskNum    = taskNumArg);
}

//
// Private helper: would the proposed placement be legal?
// Assume the existing placement is legal.
//
proc Board.nextPlacementIsLegal(row: int, col: int): bool {
  for i in 1..lastfilled {
    if
      queenvec(i) == col ||
      queenvec(i) - col == i - row ||
      queenvec(i) - col == row - i
    then {
      // it's illegal, quit
      debug("  conflict: ", i, ", ", queenvec(i), " vs ", row, ", ", col, "\n");
      return false;
    }
  }

  // everything is hunky-dory
  debug("  legal\n");
  return true;
}

//
// Display the board.
//
config var show1line: bool = true;

proc Board.show(msg...): void {
  if boardSize <= 0 then {
    writeln("the board is empty", (...msg));
    return;
  }
  var notFilledMsg = "";
  if lastfilled < boardSize then notFilledMsg =
    " row(s) "+ (lastfilled + 1):string + " to " + boardSize:string + " are not filled";
  if show1line then {
    writeln(
	    taskNum, ": ",
            [row in 1..lastfilled] (row, queenvec(row)),
            notFilledMsg, (...msg));
  } else {
    for {1..boardSize} do write("-"); writeln((...msg));
    for row in 1..lastfilled {
      for col in 1..boardSize do
        write(if queenvec(row) == col then "*" else " ");
      writeln();
    }
    if notFilledMsg != "" then writeln(notFilledMsg);
    for {1..boardSize} do write("-"); writeln();
  }
}

/////////////////////////////////////////////////////////////////////////////
// debugging support

// show debugging info?
config var dbg: bool = false;
proc debug(arg...)  { if dbg  then write((...arg)); }

// a separate flag for parallelization
config var pdbg: bool = false;
proc pdebug(arg...) { if pdbg then write((...arg)); }

/////////////////////////////////////////////////////////////////////////////
// the algorithm

// ancillary: count the solutions
var solutionCount: sync int;

// ancillary: show each solution as it is discovered?
config var showEachSolution: bool = true;

// ancillary: count the tasks; to be used from the initial thread only
var taskCount: int = 0;

// parallelization: recurse in parallel once this many rows have been filled
// (larger parRow makes it create more tasks)
// (it is 'const' so we can correctly deallocate boards in tryQueenInNextRow())
config const parRow: int = 3;

//
// Given a partially-filled board, we try placing a queen
// in the next row - trying each column in turn.
// If the column succeeds, we proceed to the next row
// (or show the result if we have filled all rows).
//
proc tryQueenInNextRow(board: unmanaged Board): void {
  // the row we will be placing in
  var nextRow = board.lastfilled + 1;

  // iterate over the columns
  for col in 1..board.boardSize {
    // place the queen in that column if legal
    if board.placeNextIfLegal(nextRow, col) then {
      if nextRow == board.boardSize then {
        // found a complete solution
        solutionCount.writeEF(solutionCount.readFE() + 1);
        if showEachSolution then board.show("");
      } else if nextRow == parRow then {
        // recurse in parallel

        // (ensure we are in the initial task)
        if !(board.taskNum == 0) then
          halt("creating a new job from taskNum ", board.taskNum,
               ", not the initial task");

        taskCount += 1;
        var mytask = taskCount;
	var clone = board.clone(mytask);

        pdebug("forking off task #", mytask, " at ", nextRow, ", ", col, "\n");
	begin tryQueenInNextRow(clone);

      } else {
        // recurse within the same task
        tryQueenInNextRow(board);
      }
      // remove the placed queen so we can try another column
      board.removeLast(nextRow, col);
    }
  }

  // deallocate the clone board if we are done with it
  if board.lastfilled == parRow then {
    pdebug("deallocating board for task #", board.taskNum, "\n");
    if !(board.taskNum > 0) then
      halt("should not be deallocating the initial task's board");
    delete board;
  }
}  // tryQueenInNextRow

/////////////////////////////////////////////////////////////////////////////
// the driver

//
// Find all solutions for the given board size, optionally showing each,
// and print their count.
//
proc countSolutions(boardSize: int, showEachSoln: bool) {
  solutionCount.writeEF(0);
  showEachSolution = showEachSoln;
  if showEachSoln then
    writeln("Solving N Queens for N=", boardSize,
            " in parallel from level ", parRow, "...");
  sync {
    var board = createBoard(boardSize);

    tryQueenInNextRow(board);   // elide dealloc of this board

    delete board;
  }
  writeln("Found ", solutionCount.readFE(), " solutions for N=", boardSize);
}

// how big the board to play
config const N = 8;

// up to how big a board to print solution counts for
config var N_counts_only = 9;

//
// The main program.
//
proc main() {
  if N > 0 then
    countSolutions(N, showEachSolution);
  for n in 1..N_counts_only do
    countSolutions(n, false);
}
