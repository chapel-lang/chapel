/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Kyle Brady and Ben Albrecht
   derived from the C++ implementation by Stefan Westen
 */

use BitOps;

param boardCells = 50,          // Number of cells on board
      boardWidth = 5,           // Number of cells along x-axis
      boardHeight = 10,         // Number of cells along y-axis
      totalMasks = 8192,        // Upper bound for allMasks needed (power of 2)
      piecePermutations = 12,   // Number of permutations for a single piece
      numPieces = 10,           // Number of puzzle pieces
      pieceCells = 5;           // Number of cells that make up a puzzle piece

const boardDom = {0..#boardCells},
      piecesDom = {0..#numPieces};

// Arrays of masks to store all pieces and their possible configurations
var allMasks: [0..#totalMasks] int,
    maskStart: [boardDom][0..#numPieces-2] int;

// Arrays of min and max, and an integer storing the number of solutions
var minSolution, maxSolution: [boardDom] int,
    solutions = 0;

// Masks for evens and left border
var evenRowsLookup, leftBorderLookup: [boardDom] int;

// Predefined masks commonly used throughout program
param maskEven   = 0xf07c1f07c1f07c1f: int, // Even rows of board (0, 2, 4, ..)
      maskBorder = 0x1084210842108421: int, // Right border of board
      maskBoard  = 0xFFFC000000000000: int, // bits of board within 64-bit int
      maskBottom = 0x00000000003FFFFF: int, // Bottom 22 elements of boards
      maskUsed   = 0x00000000FFC00000: int; // Board + 4 additional bits

//
// Find and print the minimum and maximum solutions to meteor puzzle
//
proc main(args: [] string) {

  initialize();

  sync searchParallel();

  writef("%i solutions found\n\n", solutions);
  printBoard(minSolution);
  printBoard(maxSolution);
}


//
// Set up allMasks with pre-filtered piece permutations to search
//
proc initialize() {

  // Set up array of evens and borders masks
  for i in 0..#boardCells {
    evenRowsLookup[i] = (maskEven >> i);
    leftBorderLookup[i] = (maskBorder >> i);
  }

  var totalCount = 0,
      coords: [0..#pieceCells] 2*int;

  /* The 10 puzzle pieces are defined with hexagonal cell coordinates, where
     the first cell of a piece always begins at the origin cell, (0, 0).
     This coordinate system is best conveyed through a diagram:

                         (0, -1)       (1, -1)

                  (-1, 0)       (0 , 0)       (1 , 0)

                         (-1, 1)       (0 , 1)

     Therefore, the cell to the South of the origin can be computed as follows:

        S =    SW   +   SE
        S = (-1, 1) + (0, 1) = (-1, 2)

     The following illustrates the piece shapes and their cell indices:

        Piece 0      Piece 1      Piece 2      Piece 3      Piece 4

        0 1 2 3         0          0 1 2        0 1 2        0
               4         1        3                3          1 2
                      2 3          4                4        3   4
                     4

        Piece 5      Piece 6      Piece 7      Piece 8      Piece 9

           0 1        0 1          0   1        0            0
        2 3 4          2          2 3 4          1            1
                      3                           2 3          2
                       4                             4        3 4

  */
  const pieces: [piecesDom][0..#pieceCells] 2*int =
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
    for xBase in 0..#boardWidth {
      // Compute base position for xBase, yBase coordinates
      const pos = (xBase + boardWidth * yBase);

      // Record total number of fitted pieces at this point
      maskStart[pos][0] = totalCount;

      // Loop over piece indices
      for piece in 0..#numPieces {

        // Get x y coordinates of piece cells
        for (coord, pieceCell) in zip(coords, pieces[piece]) do
          coord = pieceCell;

        // Loop over 12 permutations for a given piece (6 rotations * 2 flips)
        for currentPermutation in 0..#piecePermutations {

          // Skip piece 3 and specific permutations
          if piece != 3 || (currentPermutation/3) % 2 == 0 {

            // Serial reduction to find min coords of a given permutation
            var (yMin, xMin) = min reduce for c in coords do (c(1), c(0));

            var mask = 0,       // Bit mask representing piece's permutation
                fit = true;     // Track if piece's permutation fits board

            // Offsets for piece coordinates, such that 'island' is not formed
            const xOff = (xBase - yBase/2) - xMin,
                  yOff= yBase - yMin;

            // Set bit mask for given piece's permutation if it fits on board
            for (x, y) in coords {

              var nY = y + yOff,
                  nX = x + xOff + nY/2;

              if nX >= 0 && nX < boardWidth {
                const numBits = nX - xBase + boardWidth * (nY - yBase);
                mask |= (1 << numBits);
              } else
                fit = false;
            }

            // If it fits and is a "good piece", add it to the array of masks
            if fit {
              if goodPiece(mask, pos) {
                allMasks[totalCount] = mask | (1 << (piece + 22));
                totalCount += 1;
              }
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
  for yBase in 0..#boardHeight {
    // Skip rows 2 & 3, since we already generated their masks
    if yBase != 2 && yBase != 3 {
      for xBase in 0..#boardWidth {
        const pos = (xBase + boardWidth * yBase),
              origPos = xBase + boardWidth * (yBase % 2 + 2);
        maskStart[pos][0] = totalCount;
        var pMask = maskStart[origPos][0];
        const bottom = ((maskBoard >> pos) & maskBottom),
              lastRow = ((maskBoard >> (pos + boardWidth)) & maskBottom);
        while allMasks[pMask] {
          const mask = allMasks[pMask];
          pMask += 1;
          if (mask & bottom) == 0 {
            if (yBase == 0 || ((mask & lastRow) != 0)) &&
               !goodPiece(mask & maskBottom, pos) then
              continue;

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
    for pos in 0..#boardCells {
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
// Add initial piece to board, then fire off remaining searches in parallel
//
proc searchParallel(in board=0, in pos=0, used=0, placed=0, firstPiece=0) {

  const count = ctz(~board);
  pos += count;
  board >>= count;

  const boardAndUsed = board | used;
  var currentMask = maskStart[pos][0];

  if placed == 0 {
    while allMasks[currentMask] {
      const mask = allMasks[currentMask];
      currentMask += 1;
      searchParallel(board | (mask & maskBottom), pos,
                     used | (mask & maskUsed), placed + 1, mask);
    }
  // 1 piece has been placed
  } else {
    while allMasks[currentMask] {
      while allMasks[currentMask] & boardAndUsed do
        currentMask += 1;

      if allMasks[currentMask] {
        const mask = allMasks[currentMask];


        begin with (in board, in pos) {
          var currentSolution: [piecesDom] int =
            [firstPiece, mask, 0, 0, 0, 0, 0, 0, 0, 0];

          searchLinear(board | (mask & maskBottom), pos,
                       used | (mask & maskUsed), placed+1,
                       currentSolution);
        }

        currentMask += 1;
      }
    }
  }
}


//
// Pretty printed output of board
//
proc printBoard(board) {
  for i in 0..#boardCells {
    writef("%i ", board[i]);
    if i % boardWidth == 4 {
      writeln();
      if i & 1 == 0 then
        write(" ");
    }
  }
  writeln();
}


//
// Determine if piece and permutation (mask) at a given position (pos) is valid
//
proc goodPiece(mask, pos) {
   const evenRows = maskEven,
         oddRows = ~evenRows,
         leftBorder = maskBorder,
         rightBorder = leftBorder >> 1;

   var b = (mask << pos) | maskBoard;

   b = ~b;

   while b {
      var a = b & (-b);

      do {
        const s1 = a << 5,
              s2 = a >> 5,
              s3 = (a << 1) & (~leftBorder),
              s4 = (a >> 1) & (~rightBorder),
              s5 = ((a & evenRows) >> 6) & (~rightBorder),
              s6 = ((a & evenRows) << 4) & (~rightBorder),
              s7 = ((a & oddRows) >> 4) & (~leftBorder),
              s8 = ((a & oddRows) << 6) & (~leftBorder),
              aOld = a;
        a = (a | s1 | s2 | s3 | s4 | s5 | s6 | s7 | s8) & b;
      } while aOld != a;

      if popCount(a) % 5 != 0 then
        return false;

      b ^= a;
   }
   return true;
}


//
// Exponential backoff spin-wait lock that yields every so many contested locks
// to avoid potentially starving other tasks. This results in slightly faster
// runtime than just using a sync var because it has lower latency.
//
record BackoffSpinLock {
  var l: atomic bool,
      lockAttempts = 0,
      maxLockAttempts = (2**16-1);

  inline proc lock() {
    while l.testAndSet() {
      lockAttempts += 1;
      if (lockAttempts & maxLockAttempts) == 0 {
        maxLockAttempts >>= 1;
        chpl_task_yield();
      }
    }
  }

  inline proc unlock() {
    l.clear();
  }
}

// Minimally contended lock that serializes calls to recordSolution()
var recordSolutionLock: BackoffSpinLock;


//
// Recursively add pieces to the board, and check solution when filled
//
proc searchLinear(in board, in pos, used, placed, currentSolution) {

  if placed == numPieces {
    recordSolutionLock.lock();
    recordSolution(currentSolution);
    recordSolutionLock.unlock();
  } else {
    const evenRows = evenRowsLookup[pos],
          oddRows = ~evenRows,

          leftBorder = leftBorderLookup[pos],
          rightBorder = leftBorder >> 1,

          s1 = (board << 1) | leftBorder,
          s2 = (board >> 1) | rightBorder,
          s3 = (board << 4) | ((1 << 4) - 1) | rightBorder,
          s4 = (board >> 4) | leftBorder,
          s5 = (board << 5) | ((1 << 5) - 1),
          s6 = (board >> 5),
          s7 = (board << 6) | ((1 << 6) - 1) | leftBorder,
          s8 = (board >> 6) | rightBorder;

    if ~board & s1 & s2 & s5 & s6 &
       ((evenRows & s4 & s7) | (oddRows & s3 & s8)) then return;

    const count = ctz(~board);
    pos += count;
    board >>= count;

    const f = ((board >> 1) & 1) |
              ((board >> (4 - (evenRowsLookup[pos] & 1))) & 6),
          boardAndUsed = board | used;

    var currentMask = maskStart[pos][f];

    while allMasks[currentMask] {

      while allMasks[currentMask] & boardAndUsed do
        currentMask += 1;

      if allMasks[currentMask] {
        const mask = allMasks[currentMask];
        currentSolution[placed] = mask;
        searchLinear(board | (mask & maskBottom), pos,
                     used | (mask & maskUsed), placed + 1, currentSolution);
        currentMask += 1;
      }
    }
  }
}


//
// Check a given board as a solution and record it
//
proc recordSolution(currentSolution) {
  var board, flipBoard: [boardDom] int,
      pos, b1, count = 0;

  for i in 0..#numPieces {
    var mask = currentSolution[i];
    const piece = ctz(mask >> 22);
    mask &= maskBottom;
    b1 |= mask;
    while mask {
      const currentBit = mask & (-mask);
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
    compareSolution(board);
    compareSolution(flipBoard);
  }

  solutions += 2;
}


//
// Determine whether a solution is a min or max solution
//
proc compareSolution(board) {

  for i in 0..#boardCells {
    if board[i] < minSolution[i] {
      minSolution = board;
      break;
    } else if board[i] > minSolution[i] then break;
  }
  for i in 0..#boardCells {
    if board[i] > maxSolution[i] {
      maxSolution = board;
      break;
    } else if board[i] < maxSolution[i] then break;
  }
}

