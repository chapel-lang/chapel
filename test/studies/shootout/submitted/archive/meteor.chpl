/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Kyle Brady and Lydia Duncan
   derived from the C implementation by Christian Vosteen
 */


/* The board is a 50 cell hexagonal pattern.  For    . . . . .
   maximum speed the board will be implemented        . . . . .
   using 50 bits of a 64 bit int.                    . . . . .
                                                      . . . . .
                                                     . . . . .
   We'll represent empty cells as 0's and full        . . . . .
   cells as 1's.                                     . . . . .
                                                      . . . . .
                                                     . . . . .
                                                      . . . . .
 */

param boardCells = 50,
      boardWidth = 5,
      boardHeight = 10,
      numPieces = 10;

/* The 10 puzzle pieces are as follows:

     Piece 0   Piece 1   Piece 2   Piece 3   Piece 4

    O O O O    O   O O   O O O     O O O     O   O
           O    O O           O       O       O O
                               O         O         O

     Piece 5   Piece 6   Piece 7   Piece 8   Piece 9

      O O O     O O       O O     O O        O O O O
         O O       O O       O       O O O        O
                      O       O O

   The pieces are represented as the path followed from one end to the
   other along 12 hexagonal directions.  12 directions are used rather
   than 6 in order to fit all the piece definitions into 4-element
   arrays.  For example, it is not possible to define piece #4 in
   terms of the 6 cardinal directions using 4 moves.
 */

enum direction {
  E=0,
  ESE,
  SE,
  S,
  SW,
  WSW,
  W,
  WNW,
  NW,
  N,
  NE,
  ENE,
  PIVOT
}
use direction;

var pieceDef: [0..#numPieces] [0..3] direction
            = [[ E,  E,  E, SE],
               [SE,  E, NE,  E],
               [ E,  E, SE, SW],
               [ E,  E, SW, SE],
               [SE,  E, NE,  S],
               [ E,  E, SW,  E],
               [ E, SE, SE, NE],
               [ E, SE, SE,  W],
               [ E, SE,  E,  E],
               [ E,  E,  E, SW]];

// Max # solutions
config const n = 2100;


/* To minimize the amount of work done in the recursive solve function
   below, allocate enough space for all legal rotations of each piece
   at each position on the board. That's 10 pieces x 50 board
   positions x 12 rotations.  However, not all 12 rotations will fit
   on every cell, so keep a count of the actual number that do.
   Record the next possible open cell for each piece and location to
   reduce the burden on the solve function.
 */
var pieces, nextCell: [0..#numPieces] [0..#boardCells] [0..11] int,
    pieceCounts:      [0..#numPieces] [0..#boardCells] int,

    solutionCount: atomic int,

    solutions: [0..#n] [0..#boardCells] int,

    badEvenTriple, badOddTriple: [0..32767] bool;

proc main() {
  calcPieces();
  calcRows();
  solve();
  writeln(solutionCount.read(), " solutions found\n");
  printLargestSmallest();
}

/* Calculate every legal rotation for each piece at each board location. */
proc calcPieces() {
  var cells: [0..#numPieces] [0..4] int;

  forall piece in 0..#numPieces {
    for indx in 0..#boardCells {
      calcSixRotations(piece, indx, cells[piece]);
      serial do pieceDef[piece] = flip(pieceDef[piece]);
      calcSixRotations(piece, indx, cells[piece]);
    }
  }
}

/* Calculate all six rotations of the specified piece at the specified
   index.  Calculate only half of piece 3's rotations, because any
   solution found has an identical solution rotated 180 degrees.  Thus
   we can reduce the number of attempted pieces in the solve algorithm
   by not including the 180-degree-rotated pieces of ONE of the
   pieces.  Piece 3 was chosen because it gave the best time ;)
 */
proc calcSixRotations(piece, indx, cell) {
  for rotation in 0..5 {
    if piece != 3 || rotation < 3 {
      calcCellIndices(cell, piece, indx);
      if cellsFitOnBoard(cell, piece) && !hasIsland(cell, piece) {
        const minimum = min reduce (for i in cell do i),
              firstEmpty = firstEmptyCell(cell, minimum),
              pieceMask = | reduce (for i in cell do 1 << i);

        recordPiece(piece, minimum, firstEmpty, pieceMask);
      }
    }
    serial do pieceDef[piece] = rotate(pieceDef[piece]);
  }
}

/* Convenience function to quickly calculate all of the indices for a
   piece
 */
proc calcCellIndices(cell, piece, indx) {
  cell[0] = indx;
  for i in 0..3 do
    cell[i+1] = shift(cell[i], pieceDef[piece][i]);
}

/* Returns the new cell index from the specified cell in the
   specified direction.  The index is only valid if the starting
   cell and direction have been checked by the outOfBounds function
   first.
 */
proc shift(cell, dir) {
  select dir {
    when E do
      return cell + 1;
    when ESE do
      if (cell / 5) % 2 then
        return cell + 7;
      else
        return cell + 6;
    when SE do
      if (cell / 5) % 2 then
        return cell + 6;
      else
        return cell + 5;
    when S do
      return cell + 10;
    when SW do
      if (cell / 5) % 2 then
        return cell + 5;
      else
        return cell + 4;
    when WSW do
      if (cell / 5) % 2 then
        return cell + 4;
      else
        return cell + 3;
    when W do
      return cell - 1;
    when WNW do
      if (cell / 5) % 2 then
        return cell - 6;
      else
        return cell - 7;
    when NW do
      if (cell / 5) % 2 then
        return cell - 5;
      else
        return cell - 6;
    when N do
      return cell - 10;
    when NE do
      if (cell / 5) % 2 then
        return cell - 4;
      else
        return cell - 5;
    when ENE do
      if (cell / 5) % 2 then
        return cell - 3;
      else
        return cell - 4;
    otherwise
      return cell;
  }
}

/* Convenience function to quickly calculate if a piece fits on the board */
proc cellsFitOnBoard(cell, piece) {
  return (&& reduce (for i in 0..3 do
                       !outOfBounds(cell[i], pieceDef[piece][i])));
}

/* Returns whether the specified cell and direction will land outside of the
   board.  Used to determine if a piece is at a legal board location or not.
 */
proc outOfBounds(cell, dir) {
  select dir {
    when E do
      return cell % boardWidth == 4;
    when ESE {
      const i = cell % boardHeight;
      return i == 4 || i == 8 || i == 9 || cell >= 45;
    }
    when SE do
      return cell % boardHeight == 9 || cell >= 45;
    when S do
      return cell >= 40;
    when SW do
      return cell % boardHeight == 0 || cell >= 45;
    when WSW {
      const i = cell % boardHeight;
      return i == 0 || i == 1 || i == 5 || cell >= 45;
    }
    when W do
      return cell % boardWidth == 0;
    when WNW {
      const i = cell % boardHeight;
      return i == 0 || i == 1 || i == 5 || cell < 5;
    }
    when NW do
      return cell % boardHeight == 0 || cell < 5;
    when N do
      return cell < 10;
    when NE do
      return cell % boardHeight == 9 || cell < 5;
    when ENE {
      const i = cell % boardHeight;
      return i == 4 || i == 8 || i == 9 || cell < 5;
    }
    otherwise
      return false;
  }
}

/* To thin the number of pieces, calculate if any of them trap any
   empty cells at the edges.  There are only a handful of exceptions
   where the board can be solved with the trapped cells.  For example:
   piece 8 can trap 5 cells in the corner, but piece 3 can fit in
   those cells, or piece 0 can split the board in half where both
   halves are viable.
 */
proc hasIsland(cell, piece) {
  var tempBoard: [0..#boardCells] int,
      c = 0;

  for i in 0..4 do
    tempBoard[cell[i]] = 1;

  var i = boardCells - 1;

  while tempBoard[i] == 1 do
    i -= 1;
  fillContiguousSpace(tempBoard, i);

  for i in  0..#boardCells do
    if tempBoard[i] == 0 then
      c += 1;

  return !(c == 0 || (c == 5 && piece == 8) || (c == 40 && piece == 8) ||
           (c % 5 == 0 && piece == 0));
}

/* Fill the entire board going cell by cell.  If any cells are "trapped"
   they will be left alone.
 */
proc fillContiguousSpace(board, indx) {
  if board[indx] == 1 then
    return;

  board[indx] = 1;

  for dir in (E, SE, SW, W, NW, NE) do
    if !outOfBounds(indx, dir) then
      fillContiguousSpace(board, shift(indx, dir));
}

/* Calculate the lowest possible open cell if the piece is placed on
   the board.  Used to later reduce the amount of time searching for
   open cells in the solve function.
 */
proc firstEmptyCell(cell, in min) {
  while (min == cell[0] || min == cell[1] ||
         min == cell[2] || min == cell[3] ||
         min == cell[4]) do
    min += 1;

  return min;
}

/* Record the piece and other important information in arrays that
   will later be used by the solve function.
 */
proc recordPiece(piece, minimum, firstEmpty, pieceMask) {
  const pieceCount = pieceCounts[piece][minimum];

  pieces[piece][minimum][pieceCount] = pieceMask;
  nextCell[piece][minimum][pieceCount] = firstEmpty;
  pieceCounts[piece][minimum] += 1;
}

/* Returns the direction rotated 60 degrees clockwise */
proc rotate(dir) {
  return ((dir + 2) % PIVOT:int): direction;
}

/* Returns the direction flipped on the horizontal axis */
proc flip(dir) {
  return ((PIVOT - dir:int) % PIVOT:int): direction;
}

/* Calculate all 32 possible states for a 5-bit row and all rows that
   will create islands that follow any of the 32 possible rows.  These
   pre- calculated 5-bit rows will be used to find islands in a
   partially solved board in the solve function.
 */
proc calcRows() {
  var badEvenRows, badOddRows: [0..31] [0..31] bool;

  forall row1 in 0..31 {
    for row2 in 0..31 {
      badEvenRows[row1][row2] = rowsBad(row1, row2, true);
      badOddRows[row1][row2] = rowsBad(row1, row2, false);
    }
  }

  for row1 in 0..31 {
    for row2 in 0..31 {
      for row3 in 0..31 {
        const idx = row1 + (row2*32) + (row3*1024);
        var even = badEvenRows[row1][row2],
            odd = badOddRows[row2][row3];

        if !even && odd && tripleIsOkay(row1, row2, row3, true) then
          badEvenTriple[idx] = false;
        else
          badEvenTriple[idx] = even || odd;

        odd = badOddRows[row1][row2];
        even = badEvenRows[row2][row3];
        if !odd && even && tripleIsOkay(row1, row2, row3, false) then
          badOddTriple[idx] = false;
        else
          badOddTriple[idx] = odd || even;
      }
    }
  }
}

proc rowsBad(row1, row2, even) {
  param ROWMASK    = 0x1F;

  /* even is referring to row1 */
  var inZeroes, groupOkay = false;

  /* Test for blockages at same index and shifted index */
  const row2Shift = if even then ((row2 << 1) & ROWMASK) | 0x01
                            else (row2 >> 1) | 0x10,
        block = ((row1 ^ row2) & row2) & ((row1 ^ row2Shift) & row2Shift);

  /* Test for groups of 0's */
  for i in 0..4 {
    if row1 & (1 << i) {
      if inZeroes {
        if !groupOkay then
          return true;
        inZeroes = false;
        groupOkay = false;
      }
    } else {
      if !inZeroes then
        inZeroes = true;
      if !((block & (1 << i))) then
        groupOkay = true;
    }
  }

  if inZeroes then
    return !groupOkay;
  else
    return false;
}

/* Check for cases where three rows checked sequentially cause a false
   positive.  One scenario is when 5 cells may be surrounded where piece 5
   or 7 can fit.  The other scenario is when piece 2 creates a hook shape.
 */
proc tripleIsOkay(row1, row2, row3, even) {
  if even then
    /* There are four cases:
       row1: 00011  |  00001  |  11001  |  10101
       row2: 01011  |  00101  |  10001  |  10001
       row3: 011??  |  00110  |  ?????  |  ?????
     */
    return ((row1 == 0b00011) && (row2 == 0b01011) &&
            ((row3 & 0b11100) == 0b01100)) ||
           ((row1 == 0b00001) && (row2 == 0b00101) && (row3 == 0b00110)) ||
           ((row1 == 0b11001) && (row2 == 0b10001)) ||
           ((row1 == 0b10101) && (row2 == 0b10001));
  else
    /* There are two cases:
       row1: 10011  |  10101
       row2: 10001  |  10001
       row3: ?????  |  ?????
     */
    return ((row1 == 0b10011) && (row2 == 0b10001)) ||
           ((row1 == 0b10101) && (row2 == 0b10001));
}

/* The recursive solve algorithm.  Try to place each permutation in
   the upper- leftmost empty cell.  Mark off available pieces as it
   goes along.  Because the board is a bit mask, the piece number and
   bit mask must be saved at each successful piece placement.  This
   data is used to create a 50 char array if a solution is found.
 */
proc solve() {
  forall piece in 0..#numPieces do
    solveHelper(piece);
}

proc solveHelper(piece) {
  var board: uint = 0xFFFC000000000000, // fill bits not in the board
      avail: uint = 0x03FF,
      solNums, solMasks: [0..#numPieces] int;

  const depth, cell = 0,
        pieceNoMask = 1 << piece,
        maxRots = pieceCounts[piece][cell];

  avail ^= pieceNoMask;
  for rotation in 0..(maxRots-1) {
    if !((board & pieces[piece][cell][rotation]):bool) {
      solNums[depth] = piece;
      solMasks[depth] = pieces[piece][cell][rotation];
      board |= pieces[piece][cell][rotation];
      if !boardHasIslands(nextCell[piece][cell][rotation], board) {
        solveLinear(1, nextCell[piece][cell][rotation],
                    board, avail, solNums, solMasks);
      }
      board ^= pieces[piece][cell][rotation];
    }
  }

  avail ^= pieceNoMask;
}

/* Calculate islands while solving the board. */
proc boardHasIslands(cell, board) {
  param TRIPLEMASK = 0x7FFF;
  /* Too low on board, don't bother checking */
  if cell >= 40 then
    return false;

  const currentTriple = (board >> ((cell / 5) * 5)) & TRIPLEMASK;

  if (cell / 5) % 2 then
    return badOddTriple[currentTriple:int];
  else
    return badEvenTriple[currentTriple:int];
}

proc solveLinear(in depth, in cell, in board, in avail, solNums, solMasks) {
  if solutionCount.read() >= n then
    return;

  while (board & (1 << cell)) do
    cell += 1;

  for piece in 0..#numPieces {
    const pieceNoMask = 1 << piece;

    if !((avail & pieceNoMask):bool) then
      continue;

    avail ^= pieceNoMask;

    const maxRots = pieceCounts[piece][cell];

    for rotation in 0..(maxRots-1) {
      if !((board & pieces[piece][cell][rotation]):bool) {
        solNums[depth] = piece;
        solMasks[depth] = pieces[piece][cell][rotation];
        if depth == 9 {
          /* Solution found!!!!!11!!ONE! */
          recordSolution(solNums, solMasks);
          avail ^= pieceNoMask;
          return;
        }
        board |= pieces[piece][cell][rotation];
        if !boardHasIslands(nextCell[piece][cell][rotation], board) {
          solveLinear(depth + 1, nextCell[piece][cell][rotation],
                      board, avail, solNums, solMasks);
        }
        board ^= pieces[piece][cell][rotation];
      }
    }

    avail ^= pieceNoMask;
  }
}

proc recordSolution(solNums, solMasks) {
  const mySolCount = solutionCount.fetchAdd(2);

  for solNo in 0..#numPieces {
    var solMask = solMasks[solNo];

    for indx in 0..#boardCells {
      if solMask & 1 {
        solutions[mySolCount][indx] = solNums[solNo];
        /* Board rotated 180 degrees is a solution too! */
        solutions[mySolCount + 1][boardCells - 1 - indx] = solNums[solNo];
      }
      solMask >>= 1;
    }
  }
}

proc printLargestSmallest() {
  var sIndx, lIndx = 0;

  for i in 1..solutionCount.read()-1 {
    if solutionLessThan(lIndx, i) then
      lIndx = i;
    else if solutionLessThan(i, sIndx) then
      sIndx = i;
  }

  pretty(solutions[sIndx]);
  pretty(solutions[lIndx]);
}

proc solutionLessThan(lhs, rhs) {
  if lhs == rhs then
    return false;

  for i in 0..#boardCells do
    if solutions[lhs][i] != solutions[rhs][i] then
      return solutions[lhs][i] < solutions[rhs][i];

  return false;
}

/* pretty print a board in the specified hexagonal format */
proc pretty(s) {
  for i in 0..#boardCells {
    writef("%i ", s[i]);
    if i % boardWidth == 4 {
      writeln();
      if i & 1 == 0 then
        write(" ");
    }
  }
  writeln();
}
