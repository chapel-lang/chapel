use IO;

config const inputfile = "easy.dat";

var givenBoard: [1..9, 1..9] int;  // The given board
var board:  [1..9, 1..9] int;

proc checkForNum(num: int, board: [] int, iters: domain(2)): bool {
  for spot in iters do
    if board(spot) == num then return false;

  return true;
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
      for currentNum in 1..9 {
        //FIXME: if !numsAvail[currentNum] then continue;
        if numsAvail[currentNum] == true {

          var nSpots: int;
          spots = spotsAvail;
          for spot in 1..9 {
            if spots[spot] == true {

              // Search this column for the current num
              spots[spot] = checkForNum(currentNum, board, {1..9, spot..spot});

              // Search this block for the current num
              if spots[spot] then
                spots[spot] = checkForNum(currentNum, board, {((row-1)/3:int)*3+1..((row-1)/3:int+1)*3,
                                ((spot-1)/3:int)*3+1..((spot-1)/3:int+1)*3});

              // Is this spot still a candidate?
              if spots[spot] == true then nSpots += 1;
            }
          }

          if nSpots == 1 then
            for spot in 1..9 do
              if spots[spot] {
                board[row, spot] = currentNum;
                spotsAvail[spot] = false;
                numFilledIn += 1;
              }
        }
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
      for currentNum in 1..9 {
        if numsAvail[currentNum] == true {

          var nSpots: int;
          spots = spotsAvail;
          for spot in 1..9 {
            if spots[spot] == true {

              // Search this row for the current num
              spots[spot] = checkForNum(currentNum, board, {spot..spot, 1..9});

              // Search this block for the current num
              if spots[spot] then
                spots[spot] = checkForNum(currentNum, board, {((spot-1)/3:int)*3+1..((spot-1)/3:int+1)*3,
                                ((col-1)/3:int)*3+1..((col-1)/3:int+1)*3});

              // Still true?  Could go in this spot..
              if spots[spot] == true then nSpots += 1;
            }
          }

          if nSpots == 1 then
            for spot in 1..9 do
              if spots[spot] {
                board[spot, col] = currentNum;
                spotsAvail[spot] = false;
                numFilledIn += 1;
              }
        }
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
      for currentNum in 1..9 {
        if numsAvail[currentNum] == true {

          var nSpots: int;
          g_spots = g_spotsAvail;
          for (k, l) in {1..3, 1..3} {
            if g_spots[k, l] == true {

              // Search the current row for this num
              g_spots[k, l] = checkForNum(currentNum, board, {i*3+k..i*3+k,1..9});

              // Search the current column for this num
              if g_spots[k, l] then
                g_spots[k, l] = checkForNum(currentNum, board, {1..9,j*3+l..j*3+l});

              // Still true?  Could go in this spot..
              if g_spots[k, l] == true then nSpots += 1;
            }
          }

          if nSpots == 1 then
            for (k, l) in {1..3, 1..3} do
              if g_spots[k, l] == true {
                board[i*3+k, j*3+l] = currentNum;
                g_spotsAvail[k, l] = false;
                numFilledIn += 1;
              }
        }
      }
    }
    writeln("Blk-Wise Elimination: ", numFilledIn);
  } while numFilledIn > 0;
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

  board = givenBoard; // Set aside the initial board

  populateBoard(board);

  // Count the blanks
  var nSpots = 0;
  for (i, j) in {1..9, 1..9} {
    if board[i, j] == 0 {
      nSpots += 1;
    }
  }

  if nSpots == 0 then writeln("\nPuzzle solved:\n\n", board);
  else writeln("\nI was able to fill in all but ", nSpots, " spaces, good luck with the rest!\n\n", board);
}
