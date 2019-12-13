use Random, IO;

config const inputfile = "easy.dat";
config const ANNEAL    = 200;
config const MIN_SWAPS = 1;
config const MAX_ITER  = 500000;

var givenBoard: [1..9, 1..9] int;  // The given board
var initBoard:  [1..9, 1..9] int;  // The initialized board

var myRand = new owned RandomStream(real);


// Return a random number on the range [1, n]
proc getRand(n: int) {
  return (myRand.getNext()*max(int)):int%n+1;
}


proc populateBoard(inout board: [] int) {
  var spotsAvail:   [1..9] bool;
  var spots:        [1..9] bool;
  var g_spotsAvail: [1..3, 1..3] bool;
  var g_spots:      [1..3, 1..3] bool;
  var numsAvail:    [1..9] bool;
  var numFilledIn: int;

  do {
    numFilledIn = 0;

    // ROW-WISE ELIMINATION -- Attempt to fill in spots row-wise
    for row in 1..9 {
      spotsAvail = false;
      numsAvail  = true;

      // Find spots and nums available in this row
      for col in 1..9 {
        if board[row, col] == 0 then spotsAvail[col] = true;
        else numsAvail[board[row, col]] = false;
      }

      // For all nums available
      var currentNum = 1;
      while currentNum <= 9 {
        if numsAvail[currentNum] == true {

          var nSpots: int;
          spots = spotsAvail;
          for spot in 1..9 {
            if spots[spot] == true {

              // Search this column for the current num
              for i in 1..9 {
                if board[i, spot] == currentNum {
                  spots[spot] = false;
                  break;
                }
              }

              // Search this block for the current num
              for ij in {((row-1)/3:int)*3+1..((row-1)/3:int+1)*3,
                ((spot-1)/3:int)*3+1..((spot-1)/3:int+1)*3} {
                if board(ij) == currentNum {
                  spots[spot] = false;
                  break;
                }
              }

              // Is this spot still a candidate?
              if spots[spot] == true then nSpots += 1;
            }
          }

          if nSpots == 1 {
            for spot in 1..9 {
              if spots[spot] == true {
                board[row, spot] = currentNum;
                spotsAvail[spot] = false;
                numFilledIn += 1;
              }
            }
          }
        }

        currentNum += 1;
      }
    }
    writeln("Row-Wise Elimination: ", numFilledIn);

    // COL-WISE ELIMINATION -- Attempt to fill in spots col-wise
    for col in 1..9 {
      spotsAvail = false;
      numsAvail  = true;

      // Find spots and nums available in this column
      for row in 1..9 {
        if board[row, col] == 0 then spotsAvail[row] = true;
        else numsAvail[board[row, col]] = false;
      }

      // For all nums available
      var currentNum = 1;
      while currentNum <= 9 {
        if numsAvail[currentNum] == true {

          var nSpots: int;
          spots = spotsAvail;
          for spot in 1..9 {
            if spots[spot] == true {

              // Search this row for the current num
              for i in 1..9 {
                if board[spot, i] == currentNum {
                  spots[spot] = false;
                  break;
                }
              }

              // Search this block for the current num
              for ij in {((spot-1)/3:int)*3+1..((spot-1)/3:int+1)*3,
                ((col-1)/3:int)*3+1..((col-1)/3:int+1)*3} {
                if board(ij) == currentNum {
                  spots[spot] = false;
                  break;
                }
              }

              // Still true?  Could go in this spot..
              if spots[spot] == true then nSpots += 1;
            }
          }

          if nSpots == 1 {
            for spot in 1..9 {
              if spots[spot] == true {
                board[spot, col] = currentNum;
                spotsAvail[spot] = false;
                numFilledIn += 1;
              }
            }
          }
        }

        currentNum += 1;
      }
    }
    writeln("Col-Wise Elimination: ", numFilledIn);
    
    // BLK-WISE ELIMINATION -- Attempt to fill in spots blk-wise
    for (i, j) in {0..2, 0..2} {
      g_spotsAvail = false;
      numsAvail  = true;

      // Find spots and nums available in this block
      for (k, l) in {1..3, 1..3} {
        if board[i*3+k, j*3+l] == 0 then g_spotsAvail[k, l] = true;
        else numsAvail[board[i*3+k, j*3+l]] = false;
      }

      // For all nums available
      var currentNum = 1;
      while currentNum <= 9 {
        if numsAvail[currentNum] == true {

          var nSpots: int;
          g_spots = g_spotsAvail;
          for (k, l) in {1..3, 1..3} {
            if g_spots[k, l] == true {

              // Search the current row for this num
              for r in 1..9 {
                if board[i*3+k, r] == currentNum {
                  g_spots[k, l] = false;
                  break;
                }
              }

              // Search the current column for this num
              for c in 1..9 {
                if board[c, j*3+l] == currentNum {
                  g_spots[k, l] = false;
                  break;
                }
              }

              // Still true?  Could go in this spot..
              if g_spots[k, l] == true then nSpots += 1;
            }
          }

          if nSpots == 1 {
            for (k, l) in {1..3, 1..3} {
              if g_spots[k, l] == true {
                board[i*3+k, j*3+l] = currentNum;
                g_spotsAvail[k, l] = false;
                numFilledIn += 1;
              }
            }
          }
        }

        currentNum += 1;
      }
    }
    writeln("Blk-Wise Elimination: ", numFilledIn);
  } while numFilledIn > 0;
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

  populateBoard(initBoard);

  writeln("Populated board:\n", initBoard);
  givenBoard = initBoard;


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
    newBoard = bestBoard;
    //for (i, j) in [1..9, 1..9] do
    //  newBoard[i, j] = bestBoard[i, j];

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
