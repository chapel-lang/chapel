/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Kyle Brady
 * based upon the C implementation by Christian Vosteen
 */

module meteor {
  /* The board is a 50 cell hexagonal pattern.  For    . . . . .
   * maximum speed the board will be implemented as     . . . . .
   * 50 bits, which will fit into a 64 bit long long   . . . . .
   * int.                                               . . . . .
   *                                                   . . . . .
   * I will represent 0's as empty cells and 1's        . . . . .
   * as full cells.                                    . . . . .
   *                                                    . . . . .
   *                                                   . . . . .
   *                                                    . . . . .
   */

  var board: uint = 0xFFFC000000000000;

  /* The puzzle pieces must be specified by the path followed
   * from one end to the other along 12 hexagonal directions.
   *
   *   Piece 0   Piece 1   Piece 2   Piece 3   Piece 4
   *
   *  O O O O    O   O O   O O O     O O O     O   O
   *         O    O O           O       O       O O
   *                           O         O         O
   *
   *   Piece 5   Piece 6   Piece 7   Piece 8   Piece 9
   *
   *    O O O     O O       O O     O O        O O O O
   *       O O       O O       O       O O O        O
   *                  O       O O
   *
   * I had to make it 12 directions because I wanted all of the
   * piece definitions to fit into the same size arrays.  It is
   * not possible to define piece 4 in terms of the 6 cardinal
   * directions in 4 moves.
   */
  enum direction {
    E=0,
    ESE=1,
    SE=2,
    S=3,
    SW=4,
    WSW=5,
    W=6,
    WNW=7,
    NW=8,
    N=9,
    NE=10,
    ENE=11
  }

  use direction;  // make direction's symbols directly available to this scope

  var pieceDef: [0..9][0..3] direction = [
    [  E,  E,   E, SE],
    [ SE,  E,  NE,  E],
    [  E,  E,  SE, SW],
    [  E,  E,  SW, SE],
    [ SE,  E,  NE,  S],
    [  E,  E,  SW,  E],
    [  E, SE,  SE, NE],
    [  E, SE,  SE,  W],
    [  E, SE,   E,  E],
    [  E,  E,   E, SW]
  ];

  /* To minimize the amount of work done in the recursive solve function below,
   * I'm going to allocate enough space for all legal rotations of each piece
   * at each position on the board. That's 10 pieces x 50 board positions x
   * 12 rotations.  However, not all 12 rotations will fit on every cell, so
   * I'll have to keep count of the actual number that do.
   * The pieces are going to be unsigned long long ints just like the board so
   * they can be bitwise-anded with the board to determine if they fit.
   * I'm also going to record the next possible open cell for each piece and
   * location to reduce the burden on the solve function.
   */
  var pieces: [0..9][0..49][0..11] uint;
  var pieceCounts: [0..9][0..49] int;
  var nextCell: [0..9][0..49][0..11] uint(8);

  /* Returns the direction rotated 60 degrees clockwise */
  proc rotate(dir: direction) : direction {
    return try! ((dir:int + 2) % direction.size): direction;
  }

  /* Returns the direction flipped on the horizontal axis */
  proc flip(dir: direction) : direction {
    return try! ((direction.size - dir:int) % direction.size): direction;
  }

  /* Returns the new cell index from the specified cell in the
   * specified direction.  The index is only valid if the
   * starting cell and direction have been checked by the
   * outOfBounds function first.
   */
  proc shift(cell: int(8), dir: direction) : int(8) {
    select dir {
      when E do
        return cell + 1;
      when ESE do
        if((cell / 5) % 2) then
          return cell + 7;
        else
          return cell + 6;
      when SE do
        if((cell / 5) % 2) then
          return cell + 6;
        else
          return cell + 5;
      when S do
        return cell + 10;
      when SW do
        if((cell / 5) % 2) then
          return cell + 5;
        else
          return cell + 4;
      when WSW do
        if((cell / 5) % 2) then
          return cell + 4;
        else
          return cell + 3;
      when W do
        return cell - 1;
      when WNW do
        if((cell / 5) % 2) then
          return cell - 6;
        else
          return cell - 7;
      when NW do
        if((cell / 5) % 2) then
          return cell - 5;
        else
          return cell - 6;
      when N do
        return cell - 10;
      when NE do
        if((cell / 5) % 2) then
          return cell - 4;
        else
          return cell - 5;
      when ENE do
        if((cell / 5) % 2) then
          return cell - 3;
        else
          return cell - 4;
      otherwise
        return cell;
    }
  }

  /* Returns whether the specified cell and direction will land outside
   * of the board.  Used to determine if a piece is at a legal board
   * location or not.
   */
  proc outOfBounds(cell: int(8), dir: direction) : bool {
    var i: int(8);
    select dir {
      when E do
        return cell % 5 == 4;
      when ESE {
        i = cell % 10;
        return i == 4 || i == 8 || i == 9 || cell >= 45;
      }
      when SE do
        return cell % 10 == 9 || cell >= 45;
      when S do
        return cell >= 40;
      when SW do
        return cell % 10 == 0 || cell >= 45;
      when WSW {
        i = cell % 10;
        return i == 0 || i == 1 || i == 5 || cell >= 45;
      }
      when W do
        return cell % 5 == 0;
      when WNW {
        i = cell % 10;
        return i == 0 || i == 1 || i == 5 || cell < 5;
      }
      when NW do
        return cell % 10 == 0 || cell < 5;
      when N do
        return cell < 10;
      when NE do
        return cell % 10 == 9 || cell < 5;
      when ENE {
        i = cell % 10;
        return i == 4 || i == 8 || i == 9 || cell < 5;
      }
      otherwise
        return false;
    }
  }

  /* Rotate a piece 60 degrees clockwise */
  proc rotatePiece(piece: int) {
    for i in 0..3 do
      pieceDef[piece][i] = rotate(pieceDef[piece][i]);
  }

  /* Flip a piece along the horizontal axis */
  proc flipPiece(piece: int) {
    for i in 0..3 {
      pieceDef[piece][i] = flip(pieceDef[piece][i]);
    }
  }

  /* Convenience function to quickly calculate all of the indices for a piece */
  proc calcCellIndices(cell: [] int(8), piece: int, indx: int(8) ) {
    cell[0] = indx;
    cell[1] = shift(cell[0], pieceDef[piece][0]);
    cell[2] = shift(cell[1], pieceDef[piece][1]);
    cell[3] = shift(cell[2], pieceDef[piece][2]);
    cell[4] = shift(cell[3], pieceDef[piece][3]);
  }

  /* Convenience function to quickly calculate if a piece fits on the board */
  proc cellsFitOnBoard(cell: [] int(8), piece: int) : bool {
    return (!outOfBounds(cell[0], pieceDef[piece][0]) &&
            !outOfBounds(cell[1], pieceDef[piece][1]) &&
            !outOfBounds(cell[2], pieceDef[piece][2]) &&
            !outOfBounds(cell[3], pieceDef[piece][3]));
  }

  /* Returns the lowest index of the cells of a piece.
  * I use the lowest index that a piece occupies as the index for looking up
  * the piece in the solve function.
  */
  proc minimumOfCells(cell: [] int(8)) : int(8) {
    var minimum: int(8) = max(int(8));
    for i in cell do
      if i < minimum then minimum = i;
    return minimum;
  }

  /* Calculate the lowest possible open cell if the piece is placed on the board.
  * Used to later reduce the amount of time searching for open cells in the
  * solve function.
  */
  proc firstEmptyCell(cell: [] int(8), minimum: int(8)) {
    var firstEmpty = minimum;
    while (firstEmpty == cell[0] || firstEmpty == cell[1] ||
          firstEmpty == cell[2] || firstEmpty == cell[3] ||
          firstEmpty == cell[4]) {
      firstEmpty += 1;
    }
    return firstEmpty;
  }

  /* Generate the unsigned long long int that will later be anded with the
   * board to determine if it fits.
   */
  proc bitmaskFromCells(cell: [] int(8)) : uint(64) {
    var pieceMask: uint(64) = 0;
    for i in 0..4 {
      pieceMask |= 1:uint(64) << cell[i];
    }
    return pieceMask;
  }

  /* Record the piece and other important information in arrays that will
   * later be used by the solve function.
   */
  proc recordPiece(piece: int, minimum: int, firstEmpty: int(8), pieceMask: uint(64)) {
    pieces[piece][minimum][pieceCounts[piece][minimum]] = pieceMask;
    nextCell[piece][minimum][pieceCounts[piece][minimum]] = firstEmpty:uint(8);
    pieceCounts[piece][minimum] += 1;
  }

  /* Fill the entire board going cell by cell.  If any cells are "trapped"
   * they will be left alone.
   */
  proc fillContiguousSpace(board: [] int(8), indx: int(8)) {
    if(board[indx] == 1) then
      return;
    board[indx] = 1;
    if(!outOfBounds(indx, E)) then
      fillContiguousSpace(board, shift(indx, E));
    if(!outOfBounds(indx, SE)) then
      fillContiguousSpace(board, shift(indx, SE));
    if(!outOfBounds(indx, SW)) then
      fillContiguousSpace(board, shift(indx, SW));
    if(!outOfBounds(indx, W)) then
      fillContiguousSpace(board, shift(indx, W));
    if(!outOfBounds(indx, NW)) then
      fillContiguousSpace(board, shift(indx, NW));
    if(!outOfBounds(indx, NE)) then
      fillContiguousSpace(board, shift(indx, NE));
  }

  /* To thin the number of pieces, I calculate if any of them trap any empty
   * cells at the edges.  There are only a handful of exceptions where the
   * the board can be solved with the trapped cells.  For example:  piece 8 can
   * trap 5 cells in the corner, but piece 3 can fit in those cells, or piece 0
   * can split the board in half where both halves are viable.
   */
  var tempBoard: [0..49] int(8);
  proc hasIsland(cell: [] int(8), piece: int) : bool {
    var c: int(8);

    for i in 0..49 do
      tempBoard[i] = 0;
    for i in 0..4 do
      tempBoard[cell[i]] = 1;

    var i:int(8) = 49;
    while tempBoard[i] == 1 do
      i -= 1;
    fillContiguousSpace(tempBoard, i);

    for i in  0..49 do
      if tempBoard[i] == 0 then
        c += 1;
    if (c == 0 || (c == 5 && piece == 8) || (c == 40 && piece == 8) ||
       (c % 5 == 0 && piece == 0)) {
      return false;
    } else {
      return true;
    }
  }

  /* Calculate all six rotations of the specified piece at the specified index.
   * We calculate only half of piece 3's rotations.  This is because any solution
   * found has an identical solution rotated 180 degrees.  Thus we can reduce the
   * number of attempted pieces in the solve algorithm by not including the 180-
   * degree-rotated pieces of ONE of the pieces.  I chose piece 3 because it gave
   * me the best time ;)
   */
  proc calcSixRotations(piece: int(8), indx: int(8)) {
    var minimum, firstEmpty: int(8);
    var cell: [0..4] int(8);
    var pieceMask: uint;

    for rotation in 0..5 {
      if piece != 3 || rotation < 3 {
        calcCellIndices(cell, piece, indx);
        if cellsFitOnBoard(cell, piece) && !hasIsland(cell, piece) {
          minimum = minimumOfCells(cell);
          firstEmpty = firstEmptyCell(cell, minimum);
          pieceMask = bitmaskFromCells(cell);
          recordPiece(piece, minimum, firstEmpty, pieceMask);
        }
      }
      rotatePiece(piece);
    }
  }

  /* Calculate every legal rotation for each piece at each board location. */
  proc calcPieces() {
    for piece in 0..9:int(8) {
      for indx in 0..49:int(8) {
        calcSixRotations(piece, indx);
        flipPiece(piece);
        calcSixRotations(piece, indx);
      }
    }
  }

  /* Calculate all 32 possible states for a 5-bit row and all rows that will
  * create islands that follow any of the 32 possible rows.  These pre-
  * calculated 5-bit rows will be used to find islands in a partially solved
  * board in the solve function.
  */
  const ROWMASK: int(8) = 0x1F;
  const TRIPLEMASK = 0x7FFF;
  var allRows: [0..31] int(8) = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
  17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]:int(8);
  var badEvenRows: [0..31][0..31] bool;
  var badOddRows: [0..31][0..31] bool;
  var badEvenTriple: [0..32767] bool;
  var badOddTriple: [0..32767] bool;

  proc rowsBad(row1: int(8), row2: int(8), even: bool) : bool {
    /* even is referring to row1 */
    var inZeroes, groupOkay: bool = false;
    var block, row2Shift: int(8);
    /* Test for blockages at same index and shifted index */
    if even then
      row2Shift = ((row2 << 1) & ROWMASK) | 0x01;
    else
      row2Shift = (row2 >> 1) | 0x10;
    block = ((row1 ^ row2) & row2) & ((row1 ^ row2Shift) & row2Shift);
    /* Test for groups of 0's */
    for i in 0..4 {
      if (row1 & (1 << i)) {
        if inZeroes {
          if !groupOkay then
            return true;
          inZeroes = false;
          groupOkay = false;
        }
      } else {
        if !inZeroes then
          inZeroes = true;
        if !((block & (1 << i)):bool) then
          groupOkay = true;
      }
    }
    if inZeroes then
      return !groupOkay;
    else
      return false;
  }

  /* Check for cases where three rows checked sequentially cause a false
   * positive.  One scenario is when 5 cells may be surrounded where piece 5
   * or 7 can fit.  The other scenario is when piece 2 creates a hook shape.
   */
  proc tripleIsOkay(row1: int(8), row2: int(8), row3: int(8), even: bool) : bool {
    if even {
      /* There are four cases:
      * row1: 00011  00001  11001  10101
      * row2: 01011  00101  10001  10001
      * row3: 011??  00110  ?????  ?????
      */
      return ((row1 == 0x03) && (row2 == 0x0B) && ((row3 & 0x1C) == 0x0C)) ||
             ((row1 == 0x01) && (row2 == 0x05) && (row3 == 0x06)) ||
             ((row1 == 0x19) && (row2 == 0x11)) ||
             ((row1 == 0x15) && (row2 == 0x11));
    } else {
      /* There are two cases:
      * row1: 10011  10101
      * row2: 10001  10001
      * row3: ?????  ?????
      */
      return ((row1 == 0x13) && (row2 == 0x11)) ||
             ((row1 == 0x15) && (row2 == 0x11));
    }
  }

  proc calcRows() {
    var result1, result2: bool;
    for row1 in 0:int(8)..31 {
      for row2 in 0:int(8)..31 {
        badEvenRows[row1][row2] = rowsBad(row1, row2, true);
        badOddRows[row1][row2] = rowsBad(row1, row2, false);
      }
    }
    for row1 in 0:int(8)..31 {
      for row2 in 0:int(8)..31 {
        for row3 in 0:int(8)..31 {
          result1 = badEvenRows[row1][row2];
          result2 = badOddRows[row2][row3];
          if(result1 == false && result2 == true
          && tripleIsOkay(row1, row2, row3, true)) then
            badEvenTriple[row1+(row2:int*32)+(row3:int*1024)] = false;
          else {
            badEvenTriple[row1+(row2:int*32)+(row3:int*1024)] = result1 || result2;
          }

          result1 = badOddRows[row1][row2];
          result2 = badEvenRows[row2][row3];
          if(result1 == false && result2 == true
          && tripleIsOkay(row1, row2, row3, false)) then
            badOddTriple[row1+(row2:int*32)+(row3:int*1024)] = false;
          else
            badOddTriple[row1+(row2:int*32)+(row3:int*1024)] = result1 || result2;
        }
      }
    }
  }

  /* Calculate islands while solving the board.
   */
  proc boardHasIslands(cell: uint(8)) : bool {
    /* Too low on board, don't bother checking */
    if cell >= 40 then
      return false;
    var currentTriple = (board >> ((cell / 5) * 5)) & TRIPLEMASK;
    if (cell / 5) % 2 then
      return badOddTriple[currentTriple:int];
    else
      return badEvenTriple[currentTriple:int];
  }

  /* The recursive solve algorithm.  Try to place each permutation in the upper-
   * leftmost empty cell.  Mark off available pieces as it goes along.
   * Because the board is a bit mask, the piece number and bit mask must be saved
   * at each successful piece placement.  This data is used to create a 50 char
   * array if a solution is found.
   */
  var avail: uint(16) = 0x03FF;
  var solNums: [0..9] uint(8);
  var solMasks: [0..9] uint;
  var solutions: [0..2099][0..49] uint(8);
  var solutionCount = 0;
  var maxSolutions = 2100;

  proc recordSolution() {
    var solMask: uint;
    for solNo in 0..9 {
      solMask = solMasks[solNo];
      for indx in 0..49 {
        if (solMask & 1) {
          solutions[solutionCount][indx] = solNums[solNo];
          /* Board rotated 180 degrees is a solution too! */
          solutions[solutionCount+1][49-indx] = solNums[solNo];
        }
        solMask = solMask >> 1;
      }
    }
    solutionCount += 2;
  }

  proc solve(in depth: int, in cell: int) {
    var pieceNoMask: uint(16);
    var maxRots: int;
    var pieceMask: uint;

    if solutionCount >= maxSolutions then
      return;

    while (board & (1 << cell)) do
      cell += 1;

    for piece in 0..9:uint(8) {
      pieceNoMask = 1:uint(16) << piece;
      if !((avail & pieceNoMask):bool) {
        continue;
      }
      avail ^= pieceNoMask;
      maxRots = pieceCounts[piece][cell];
      for rotation in 0..(maxRots-1) {
        if !((board & pieces[piece][cell][rotation]):bool) {
          solNums[depth] = piece;
          solMasks[depth] = pieces[piece][cell][rotation];
          if depth == 9 {
            /* Solution found!!!!!11!!ONE! */
            recordSolution();
            avail ^= pieceNoMask;
            return;
          }
          board |= pieces[piece][cell][rotation];
          if !boardHasIslands(nextCell[piece][cell][rotation]) {
            solve(depth + 1, nextCell[piece][cell][rotation]);
          }
          board ^= pieces[piece][cell][rotation];
        }
      }
      avail ^= pieceNoMask;
    }
  }

  /* pretty print a board in the specified hexagonal format */
  proc pretty(s: [] uint(8)) {
    for i in 0..49 by 10 {
      // '0' -> 48 in ascii: shifting the numbers up into valid range
      writef("%c %c %c %c %c \n %c %c %c %c %c \n", s[i]+48, s[i+1]+48,
        s[i+2]+48, s[i+3]+48, s[i+4]+48, s[i+5]+48, s[i+6]+48,
        s[i+7]+48, s[i+8]+48, s[i+9]+48);
    }
    writeln("");
  }

  proc solutionLessThan(lhs: int, rhs: int) : bool
  {
    if lhs == rhs then return false;
    for i in 0..49 {
      if solutions[lhs][i] != solutions[rhs][i] then
        return solutions[lhs][i] < solutions[rhs][i];
    }
    return false;
  }

  proc printLargestSmallest() {
    var sIndx = 0;
    var lIndx = 0;
    for i in 1..solutionCount-1 {
      if solutionLessThan(lIndx, i) {
        lIndx = i;
      } else if solutionLessThan(i, sIndx) {
        sIndx = i;
      }
    }
    pretty(solutions[sIndx]);
    pretty(solutions[lIndx]);
  }

  proc main(args: [] string) {
    if args.domain.size > 1 then
        maxSolutions = try! args[1]:int;
    calcPieces();
    calcRows();
    solve(0, 0);
    writeln(solutionCount, " solutions found\n");
    printLargestSmallest();
  }
}
