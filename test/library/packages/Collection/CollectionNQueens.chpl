use Barriers;
use DistributedBag;
use DistributedDeque;

/*
Precomputed number of solutions...

N x N | Total | Unique
1 x 1 	 1 	 1
2 x 2 	 0 	 0
3 x 3 	 0 	 0
4 x 4 	 2 	 1
5 x 5 	 10 	 2
6 x 6 	 4 	 1
7 x 7 	 40 	 6
8 x 8 	 92 	 12
9 x 9 	 352 	 46
10 x 10 	 724 	 92
11 x 11 	 2,680 	 341
12 x 12 	 14,200 	 1,787
13 x 13 	 73,712 	 9,233
14 x 14 	 365,596 	 45,752
15 x 15 	 2,279,184 	 285,053
16 x 16 	 14,772,512 	 1,846,955
17 x 17 	 95,815,104 	 11,977,939
18 x 18 	 666,090,624 	 83,263,591
19 x 19 	 4,968,057,848 	 621,012,754
20 x 20 	 39,029,188,884 	 4,878,666,808
21 x 21 	 314,666,222,712 	 39,333,324,973
22 x 22 	 2,691,008,701,644 	 336,376,244,042
23 x 23 	 24,233,937,684,440 	 3,029,242,658,210
24 x 24 	 227,514,171,973,736 	 28,439,272,956,934
25 x 25 	 2,207,893,435,808,350 	 275,986,683,743,434
26 x 26 	 22,317,699,616,364,000 	 2,789,712,466,510,280 */


config param nQueens = 8;
config param isDeque = false;
config param isBag = false;

inline proc getTotalSolutions() {
  var retval = 0;
  select nQueens {
    when 1 do retval = 1;
    when 2 do retval = 0;
    when 3 do retval = 0;
    when 4 do retval = 2;
    when 5 do retval = 10;
    when 6 do retval = 4;
    when 7 do retval = 40;
    when 8 do retval = 92;
    when 9 do retval = 352;
    when 10 do retval = 724;
    when 11 do retval = 2680;
    when 12 do retval = 14200;
    when 13 do retval = 73712;
    when 14 do retval = 365596;
    when 15 do retval = 2279184;
    when 16 do retval = 14722512;
    when 17 do retval = 95815104;
    when 18 do retval = 666090624;
    otherwise do halt("Bad parameter for nQueens: ", nQueens, "; Must be in 1 .. 18");
  }

  return retval;
}

proc findFirstEmptyRow(board) {
  var idx = 1;
  for row in board {
    if idx > nQueens {
      return nQueens + 1;
    }
    if row == 0 {
      return idx;
    }
    idx = idx + 1;
  }

  return nQueens + 1;
}

proc canPlaceQueen(board, row, col) {
  // If at any row, the column is taken, we can't occupy the slot...
  for occupiedCol in board {
    if occupiedCol == col then return false;
  }

  // Horizontal - Left/Right
  var x = 1;
  while x <= nQueens {
    if board[x-1] == col then return false;
    x = x + 1;
  }

  // Note: We do not need to check vertical, as it is ensured already to be vacant

  // Diagonal - Up Right
  x = row + 1;
  var y = col + 1;
  while x <= nQueens && y <= nQueens {
    if board[x-1] == y then return false;

    x = x + 1;
    y = y + 1;
  }

  // Diagonal - Down Left
  x = row - 1;
  y = col - 1;
  while x > 0 && y > 0 {
    if board[x-1] == y then return false;
    x = x - 1;
    y = y - 1;
  }

  // Diagonal - Up Left
  x = row - 1;
  y = col + 1;
  while x > 0 && y <= nQueens {
    if board[x-1] == y then return false;
    x = x - 1;
    y = y + 1;
  }

  // Diagonal - Down Right
  x = row + 1;
  y = col - 1;
  while x <= nQueens && y > 0 {
    if board[x-1] == y then return false;
    x = x + 1;
    y = y - 1;
  }

  return true;
}




type boardType = nQueens * int;
const totalSolutions = getTotalSolutions();
var found : atomic int;
var c = (
  if isDeque then new DistDeque(boardType)
  else if isBag then new DistBag(boardType)
  else compilerError("Require 'isDeque' or 'isBag' to be set...")
);

// Fill with initial board...
c.add(_defaultOf(boardType));

// Begin concurrent NQueens...
var barrier = new Barrier(here.maxTaskPar * numLocales);
coforall loc in Locales do on loc {
  coforall tid in 0 .. #here.maxTaskPar {
    barrier.barrier();
    var nSpins : int;
    while found.read() < totalSolutions {
      var (exists, myBoard) = c.remove();

      // Spin: We haven't found solution yet...
      if !exists {
        nSpins = nSpins + 1;
        if nSpins == 1000000 {
          halt("Spun: ", nSpins);
        }

        chpl_task_yield();
        continue;
      }

      nSpins = 0;

      const firstEmptyRow = findFirstEmptyRow(myBoard);
      if firstEmptyRow == nQueens+1 {  //no empty row
        // We are done: Alert everyone that it is done (including ourselves...)
        found.add(1);
        continue;
      }
      for j in 1..nQueens {
        if canPlaceQueen(myBoard, firstEmptyRow, j) {
          var newBoard = myBoard;
          newBoard[firstEmptyRow-1] = j;
          c.add(newBoard);
        }
      }
    }
  }
}

writeln("SUCCESS");
