/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Kyle Brady and Ben Albrecht
 * derived from the C++ implementation by Stefan Westen which states it was
 * loosely based on Ben St. John's and Kevin Barnes' implementation
 */

use BitOps;

/*
   TODO

 * More comments
 * More inferred types
 * Top down view -> Breadth first

*/

// Reusable domains
const boardDom = {0..49},
      permutationsDom = {0..8191},
      piecesDom = {0..9};

// Arrays of masks to store all pieces and their possible configurations
var allMasks: [permutationsDom] int,
    maskStart: [boardDom][0..7] int;

// Arrays of solutions
var minSolution: [boardDom] int,
    maxSolution: [boardDom] int,
    solutions: int;

// Masks for evens and left border
var evenRowsLookup: [boardDom] int,
    leftBorderLookup: [boardDom] int;

// Predefined masks commonly used throughout program
param maskEven   = 0xf07c1f07c1f07c1f: int, // Even rows of board (0, 2, 4, ..)
      maskBorder = 0x1084210842108421: int, // Right border of board
      maskBoard  = 0xFFFC000000000000: int, // bits of board within 64-bit int
      maskBottom = 0x00000000003FFFFF: int, // Bottom 22 elements of boards
      maskUsed   = 0x00000000FFC00000: int; // Board + 4 additional bits

//
// Determine if piece and permutation (mask) at a given position (pos) is valid
//
proc goodPiece(mask, pos) {
   var isGood = true;
   const evenRows = maskEven,
         oddRows = ~evenRows,
         leftBorder = maskBorder,
         rightBorder = leftBorder >> 1;
   var a, b, aOld, s1, s2, s3, s4, s5, s6, s7, s8: int;

   b = (mask << pos) | maskBoard;

   b = ~b;

   while b {
      a = b & (-b);

      do {
         s1 = a << 5;
         s2 = a >> 5;
         s3 = (a << 1) & (~leftBorder);
         s4 = (a >> 1) & (~rightBorder);
         s5 = ((a & evenRows) >> 6) & (~rightBorder);
         s6 = ((a & evenRows) << 4) & (~rightBorder);
         s7 = ((a & oddRows) >> 4) & (~leftBorder);
         s8 = ((a & oddRows) << 6) & (~leftBorder);
         aOld = a;
         a = (a | s1 | s2 | s3 | s4 | s5 | s6 | s7 | s8) & b;
      } while aOld != a;

      if popcount(a) % 5 != 0 {
         isGood = false;
         break;
      }
      b ^= a;
   }
   return isGood;
}

//
// Setup allMasks with pre-filtered piece permutations to search
//
proc initialize() {

  // Set up array of evens and borders masks
  for i in boardDom {
    evenRowsLookup[i] = (maskEven >> i);
    leftBorderLookup[i] = (maskBorder >> i);
  }

  var totalCount = 0,
      coords: [0..4] 2*int;

  const pieceCount = 10;

  /* The puzzle pieces are defined with hexagonal cell coordinates
   * starting from the origin [0, 0].
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
   */
  const pieces: [piecesDom][0..4] 2*int =
  [
    [(0, 0), (1, 0), ( 2, 0), ( 3, 0), ( 3, 1)],
    [(0, 0), (0, 1), (-2, 2), (-1, 2), (-3, 3)],
    [(0, 0), (1, 0), ( 2, 0), (-1, 1), (-1, 2)],
    [(0, 0), (1, 0), ( 2, 0), ( 1, 1), ( 1, 2)],
    [(0, 0), (0, 1), ( 1, 1), (-1, 2), ( 1, 2)],
    [(0, 0), (1, 0), (-2, 1), (-1, 1), ( 0, 1)],
    [(0, 0), (1, 0), ( 0, 1), (-1, 2), (-1, 3)],
    [(0, 0), (2, 0), (-1, 1), ( 0, 1), ( 1, 1)],
    [(0, 0), (0, 1), ( 0, 2), ( 1, 2), ( 1, 3)],
    [(0, 0), (0, 1), ( 0, 2), (-1, 3), ( 0, 3)]
  ];

  // Generate bit mask for permutations of all pieces that fit on rows 2 & 3
  for yBase in 2..3 {
    for xBase in 0..4 {
      // Compute base position for xBase, yBase coordinates
      const pos = (xBase + 5 * yBase);

      // Record total number of fitted pieces at this point
      maskStart[pos][0] = totalCount;

      // Loop over piece indices
      for piece in 0..pieceCount-1 {

        // Get x y coordinates of piece cells
        for (coord, pieceCell) in zip(coords, pieces[piece]) do
          coord = pieceCell;

        // Loop over 12 permutations for a given piece (6 rotations * 2 flips)
        for currentPermutation in 0..11 {

          // Skip piece 3 and specific permutations
          if piece != 3 || (currentPermutation/3) % 2 == 0 {

            // Compute the xMin and yMin of coordinates for given permutation
            var xMin = coords[0][1], yMin = coords[0][2];
            for (x, y) in coords {
              if y < yMin || (y == yMin && x < xMin) then
                (xMin, yMin) = (x, y);
            }

            var mask: int, // Bit mask representing piece's permutation
                fit = true;     // Track if piece's permutation fits board

            // Offsets for piece coordinates, such that 'island' is not formed
            const xOff = (xBase - yBase/2) - xMin,
                  yOff= yBase - yMin;

            // Set bit mask for given piece's permutation if it fits on board
            for (x, y) in coords {

              var nY = y + yOff,
                  nX = x + xOff + nY/2;

              if nX >= 0 && nX < 5 {
                var numBits = nX - xBase + 5 * (nY - yBase);
                mask |= (1 << numBits);
              } else
                fit = false;
            }

            // If it fits and is a "good piece", add it to the array of masks
            if fit then
              if goodPiece(mask, pos) {
                allMasks[totalCount] = mask | (1 << (piece + 22));
                totalCount += 1;
              }
          }

          // Permute piece for next iteration
          for (x, y) in coords {
            // Rotation
            (x, y) = (x + y, -x);

            // Reflection
            if currentPermutation == 5 then
              (x, y) = (x + y, -y);
          }
        } // permutations
      } // pieces

      allMasks[totalCount] = 0;
      totalCount += 1;
    } // xBase
  } // yBase

  // Generate bit masks for translation of permutations that fit on rows 2 & 3
  for yBase in piecesDom {
    // Skip rows 2 & 3, since we already generated their masks
    if yBase != 2 && yBase != 3 {
      for xBase in 0..4 {
        const pos = (xBase + 5 * yBase),
              origPos = xBase + 5 * (yBase % 2 + 2);
        maskStart[pos][0] = totalCount;
        var pMask = maskStart[origPos][0];
        const bottom = ((maskBoard >> pos) & maskBottom),
              lastRow = ((maskBoard >> (pos + 5)) & maskBottom);
        while allMasks[pMask] {
          var mask = allMasks[pMask];
          pMask += 1;
          if (mask & bottom) == 0 {
            if yBase == 0 || ((mask & lastRow) != 0) {
              if !goodPiece(mask & maskBottom, pos) {
                continue;
              }
            }
            allMasks[totalCount] = mask;
            totalCount += 1;
          }
        }
        allMasks[totalCount] = 0;
        totalCount += 1;
      }
    }
  }

  for filter in 1..7 {
    for pos in boardDom {
      maskStart[pos][filter] = totalCount;
      const filterMask = ((filter & 1) << 1) |
                         ((filter & 6) << (4 - (evenRowsLookup[pos] & 1)));
      var pMask = maskStart[pos][0];
      while allMasks[pMask] {
        const mask = allMasks[pMask];
        if (mask & filterMask) == 0 {
          allMasks[totalCount] = mask;
          totalCount += 1;
        }
        pMask += 1;
      }
      allMasks[totalCount] = 0;
      totalCount += 1;
    }
  }
}

//
// Determine whether a solution is a min or max solution
//
proc compareSolution(board, minSolution, maxSolution) {

  for i in boardDom {
    if board[i] < minSolution[i] {
      minSolution = board;
      break;
    } else if board[i] > minSolution[i] then break;
  }
  for i in boardDom {
    if board[i] > maxSolution[i] {
      maxSolution = board;
      break;
    } else if board[i] < maxSolution[i] then break;
  }
}

//
// Pretty printed output of board
//
proc printBoard(board) {
  for i in boardDom {
    writef("%i ", board[i]);
    if i%5 == 4 {
      writeln();
      if i&1 == 0 then
        write(" ");
    }
  }
  writeln();
}

//
// Check a given board as a solution and record it
//
proc recordSolution(currentSolution) {
  var board: [boardDom] int,
      flipBoard: [boardDom] int,
      mask, pos, currentBit, b1, count, piece: int;

  for i in piecesDom {
    mask = currentSolution[i];
    piece = ctz(mask >> 22);
    mask &= maskBottom;
    b1 |= mask;
    while mask {
      currentBit = mask & (-mask);
      count = ctz(currentBit);
      board[count + pos] = piece;
      flipBoard[49 - count - pos] = piece;
      mask ^= currentBit;
    }
    count = ctz(~b1);
    pos += count;
    b1 >>= count;
  }
  if solutions == 0 {
    minSolution = board;
    maxSolution = board;
  } else {
    compareSolution(board, minSolution, maxSolution);
    compareSolution(flipBoard, minSolution, maxSolution);
  }

  solutions += 2;
}

//
// DIY sync variable functionality that outperforms native sync variables
//
var l: atomic bool;

proc lock() {
  while l.testAndSet() != false do chpl_task_yield();
}

proc unlock() {
  l.write(false);
}

//
// Recursively add pieces to the board, and check solution when filled.
//
proc searchLinear(in board, in pos, in used, in placed, currentSolution) {
  var count, evenRows, oddRows, leftBorder, rightBorder,
      s1, s2, s3, s4, s5, s6, s7, s8: int;
  if placed == 10 {
    lock();
    recordSolution(currentSolution);
    unlock();
  } else {
    evenRows = evenRowsLookup[pos];

    oddRows = ~evenRows;

    leftBorder = leftBorderLookup[pos];
    rightBorder = leftBorder >> 1;

    s1 = (board << 1) | leftBorder;
    s2 = (board >> 1) | rightBorder;
    s3 = (board << 4) | ((1 << 4) - 1) | rightBorder;
    s4 = (board >> 4) | leftBorder;
    s5 = (board << 5) | ((1 << 5) - 1);
    s6 = (board >> 5);
    s7 = (board << 6) | ((1 << 6) - 1) | leftBorder;
    s8 = (board >> 6) | rightBorder;

    if ~board & s1 & s2 & s5 & s6 &
       ((evenRows & s4 & s7) | (oddRows & s3 & s8)) then return;

    count = ctz(~board);
    pos += count;
    board >>= count;

    const f = ((board >> 1) & 1) |
              ((board >> (4 - (evenRowsLookup[pos] & 1))) & 6),
          boardAndUsed = board | used;

    var mask: int,
        currentMask = maskStart[pos][f];

    while allMasks[currentMask] {

      while allMasks[currentMask] & boardAndUsed do
        currentMask += 1;

      if allMasks[currentMask] {
        mask = allMasks[currentMask];
        currentSolution[placed] = mask;
        searchLinear(board | (mask & maskBottom), pos, used | (mask & maskUsed),
                     placed + 1, currentSolution);
        currentMask += 1;
      }
    }
  }
}

//
// Wrapper to setup initial call to recursive searchLinear
//
proc searchLinearHelper(in board, in pos, in used, in placed,
                        in firstPiece, in mask) {
  var currentSolution: [piecesDom] int;
  currentSolution[0] = firstPiece;
  currentSolution[1] = mask;
  begin searchLinear(board, pos, used, placed, currentSolution);
}


//
// Add initial piece to board, then fire off remaining searches in parallel
//
proc searchParallel(in board, in pos, in used, in placed, in firstPiece) {

  var count = ctz(~board);
  pos += count;
  board >>= count;

  const boardAndUsed = board | used;
  var currentMask = maskStart[pos][0],
      mask: int;

  if placed == 0 {
    while allMasks[currentMask] {
      if allMasks[currentMask] {
        mask = allMasks[currentMask];
        currentMask += 1;
        searchParallel(board | (mask & maskBottom), pos,
                       used | (mask & maskUsed), placed + 1, mask);
      }
    }
  // 1 piece has been placed
  } else {
    while allMasks[currentMask] {
      while allMasks[currentMask] & boardAndUsed do
        currentMask += 1;

      if allMasks[currentMask] {
        mask = allMasks[currentMask];
        currentMask += 1;
        searchLinearHelper(board | (mask & maskBottom), pos,
                           used | (mask & maskUsed), placed+1,
                           firstPiece, mask);
      }
    }
  }
}


//
// Find and print the minimum and maximum solutions to meteor puzzle
//
proc main() {

  initialize();

  sync searchParallel(0, 0, 0, 0, 0);

  writef("%i solutions found\n\n", solutions);
  printBoard(minSolution);
  printBoard(maxSolution);
}
