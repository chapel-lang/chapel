use Random, IO;

config const inputfile = "easy.dat";
config const ANNEAL    = 200;
config const MIN_SWAPS = 1;
config const MAX_ITER  = 1000000;

var givenBoard: [1..9, 1..9] int;  // The given board
var initBoard:  [1..9, 1..9] int;  // The initialized board

var myRand = new owned NPBRandomStream(real, seed=314159265);


// Return a random number on the range [1, n]
proc getRand(n: int) {
  return (myRand.getNext()*max(int)):int%n+1;
}


// How many times does a number appear more than once in 
// each row, column, and block?
var seenItX: [1..9] bool = false;
var seenItY: [1..9] bool = false;
var seenIt:  [1..9] bool = false;
proc costFcn(board: [] int): int {
  var cost: int;
  
  for i in 1..9 {
    seenItX = false;
    seenItY = false;
    for j in 1..9 {
      // Check rows
      if seenItX[board[i, j]] == true then
        cost += 1;
      else seenItX[board[i, j]] = true;

      // Check cols
      if seenItY[board[j, i]] == true then
        cost += 1;
      else seenItY[board[j, i]] = true;
    }
  }

  // Check sub-blocks
  for (i, j) in {0..2, 0..2} {
    seenIt = false;
    for (k, l) in {i*3+1..(i+1)*3, j*3+1..(j+1)*3} {
      if seenIt[board[k, l]] == true then
        cost += 1;
      else seenIt[board[k, l]] = true;
    }
  }

  return cost;
}


proc main() {
  var boardData = open(inputfile, iomode.r).reader();

  // Read the board from the input file

  writeln("Reading board from: ", inputfile, "\n");
  for ij in givenBoard.domain do {
    var x: int;
    boardData.read(x);
    givenBoard(ij) = x;
  }
  
  boardData.close();
  writeln(givenBoard, "\n");

  initBoard = givenBoard; // Set aside the initial board


  // Initialize search space with all the missing digits
  //   It is invariant that the board contains 9 of each digit.
  //   Just perhaps not all in the right spots.
  var nSpots = 0;
  for i in 1..9 {
    var seenIt: [0..9] bool = false;
    for j in 1..9 do
      seenIt[initBoard[i, j]] = true;

    var j = 1, k = 1;
    while j <= 9 && k <= 9 {
      if initBoard[i, j] == 0 && seenIt[k] == false {
        initBoard[i, j] = k;
        seenIt[k] = true;
        nSpots += 1;  // Count the blanks
      }
      if initBoard[i, j] != 0 then j += 1;
      if seenIt[k] != false then k += 1;
    }
  }

  //writeln("Initialized to:\n", initBoard);
  writeln("Board has ", nSpots, " open spots");
  writeln("Initial Cost: ", costFcn(initBoard), "\n");

  // Generate array of swappable spots
  var spots: [1..nSpots] (int, int);

  var spotsUsed = 1;
  for ij in givenBoard.domain do
    if givenBoard(ij) == 0 {
      spots[spotsUsed] = ij;
      spotsUsed += 1;
    }

  // Perform the search
  var x, y: (int, int);  // Coords that will be swapped
  var bestBoard= initBoard;
  var newBoard = bestBoard;  // Working copy
  var cost     = costFcn(newBoard);
  var nSwaps   = cost**2/ANNEAL+MIN_SWAPS;
  var nIter:   int;
  var newCost: int;
  while (cost > 0) {
    // Reset to last best state
    for ij in bestBoard.domain do
      newBoard(ij) = bestBoard(ij);
    //newBoard = bestBoard;
    nIter += 1;

    for i in 1..nSwaps {
      x = spots[getRand(nSpots)];
      y = spots[getRand(nSpots)];

      newBoard(x) <=> newBoard(y);
    }

    newCost = costFcn(newBoard);
    // This board is better than our current best
    if (newCost <= cost) {
      if (newCost < cost) {
        nSwaps = newCost**2/ANNEAL+MIN_SWAPS;
        nSwaps += if nSwaps % 2 == 0 then 1 else 0;  // Make it odd
        nIter  = 0;
        writeln("Cost: ", cost, " nSwaps: ", nSwaps);
      }
      cost  = newCost;
      bestBoard = newBoard;  // Store best seen board
    }

    // Too long without an update, reset the search
    else if (nIter > MAX_ITER/cost) {
      bestBoard = initBoard;
      cost      = costFcn(bestBoard);
      nSwaps    = cost**2/ANNEAL+MIN_SWAPS;
      writeln(" **reset** Cost: ", cost, " nSwaps: ", nSwaps);
      nIter = 0;
    }
  }

  writeln("\nSolution:\n", bestBoard);
}
