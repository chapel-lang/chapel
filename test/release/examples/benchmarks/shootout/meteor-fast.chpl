/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Kyle Brady and Ben Albrecht
 * derived from the C++ implementation by Stefan Westen which states it was
 * loosely based on Ben St. John's and Kevin Barnes' implementation
 */

use BitOps;

// Arrays of masks to store all pieces and their possible configurations
var allMasks: [0..8191] uint(32),
    maskStart: [0..49][0..7] uint(32);

// Arrays of solutions
var minSolution: [0..49] uint(8),
    maxSolution: [0..49] uint(8),
    solutions: uint(32);

// Masks for evens and left border
var evenRowsLookup: [0..49] uint(32),
    leftBorderLookup: [0..49] uint(32);

// Predefined masks commonly used throughout program
param maskEven = 0xf07c1f07c1f07c1f,    // Even rows of board (rows 0, 2, 4,..)
      maskBorder = 0x1084210842108421,  // Right border of board
      maskBoard = 0xFFFC000000000000,   // bits of board within 64-bit int
      maskBottom = 0x003FFFFF,          // Bottom 22 elements of boards
      maskUsed = 0xFFC00000;            // Board + 4 additional bits

//
// Determine if piece and permutation (mask) at a given position (pos) is valid
//
proc goodPiece(mask: uint(32), pos: uint(32)): bool {
   var isGood = true;
   const evenRows: uint = maskEven,
         oddRows: uint = ~evenRows,
         leftBorder: uint = maskBorder,
         rightBorder: uint = leftBorder >> 1;
   var a, b, aOld, s1, s2, s3, s4, s5, s6, s7, s8: uint;

   b = (mask:uint << pos) | maskBoard;

   b = ~b;

   while b {
      a = b & (-(b:int)):uint;

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
         a = (a|s1|s2|s3|s4|s5|s6|s7|s8) & b;
      } while aOld != a;

      if popcount(a)%5 != 0 {
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
  for i in 0..49 {
    evenRowsLookup[i] = (maskEven >> i): uint(32);
    leftBorderLookup[i] = (maskBorder >> i): uint(32);
  }

  var totalCount: uint(32),
      xCoords: [0..4] int,
      yCoords: [0..4] int;

  const pieceCount = 10;

  /* The hexagonal puzzle pieces are defined with cell coordinates
   * starting from the origin [0,0].
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
  const pieces: [0..9][0..4][0..1] int =
  [
    [[0, 0], [1, 0], [2, 0], [3, 0], [3, 1]],
    [[0, 0], [0, 1], [-2, 2], [-1, 2], [-3, 3]],
    [[0, 0], [1, 0], [2, 0], [-1, 1], [-1, 2]],
    [[0, 0], [1, 0], [2, 0], [1, 1], [1, 2]],
    [[0, 0], [0, 1], [1, 1], [-1, 2], [1, 2]],
    [[0, 0], [1, 0], [-2, 1], [-1, 1], [0, 1]],
    [[0, 0], [1, 0], [0, 1], [-1, 2], [-1, 3]],
    [[0, 0], [2, 0], [-1, 1], [0, 1], [1, 1]],
    [[0, 0], [0, 1], [0, 2], [1, 2], [1, 3]],
    [[0, 0], [0, 1], [0, 2], [-1, 3], [0, 3]]
  ];

  // Generate bit mask for permutations of all pieces that fit on rows 2 & 3
  for yBase in 2..3 {
    for xBase in 0..4 {
      // Compute base position for xBase, yBase coordinates
      const pos: uint(32) = (xBase + 5 * yBase): uint(32);

      // Record total number of fitted pieces at this point
      maskStart[pos][0] = totalCount;

      // Loop over piece indices
      for piece in 0..pieceCount-1 {

        // Get x y coordinates of piece cells
        for j in 0..4 {
          xCoords[j] = pieces[piece][j][0];
          yCoords[j] = pieces[piece][j][1];
        }

        // Loop over 12 permutations for a given piece (6 rotations * 2 flips)
        for currentPermutation in 0..11 {

          // Skip piece 3 and specific permutations
          if piece != 3 || (currentPermutation/3) % 2 == 0 {

            // Compute the xMin and yMin of coordinates for given permutation
            var xMin = xCoords[0], yMin = yCoords[0];
            for (x, y) in zip(xCoords, yCoords) {
              if y < yMin || (y == yMin && x < xMin) then
                (xMin, yMin) = (x, y);
            }

            var mask: uint(32), // Bit mask representing piece's permutation
                fit = true;     // Track if piece's permutation fits board

            // Offsets for piece coordinates, such that 'island' is not formed
            const xOff = (xBase - yBase/2) - xMin,
                  yOff= yBase - yMin;

            // Set bit mask for given piece's permutation if it fits on board
            for (x, y) in zip(xCoords, yCoords) {

              var nY = y + yOff,
                  nX = x + xOff + nY/2;

              if nX >= 0 && nX < 5 {
                var numBits = nX - xBase + 5 * (nY - yBase);
                mask |= (1:uint(32) << numBits);
              } else
                fit = false;
            }

            // If it fits and is a "good piece", add it to the array of masks
            if fit then
              if goodPiece(mask, pos) {
                allMasks[totalCount] = mask|(1:int(32) << (piece + 22));
                totalCount += 1;
              }
          }

          // Permute piece for next iteration
          for (x, y) in zip(xCoords, yCoords) {
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
  for yBase in 0..9 {
    // Skip rows 2 & 3, since we already generated their masks
    if yBase!=2 && yBase!=3 {
      for xBase in 0..4 {
        const pos = (xBase + 5 * yBase): uint(32),
              origPos = xBase + 5 * (yBase%2 + 2);
        maskStart[pos][0] = totalCount;
        var pMask = maskStart[origPos][0];
        const bottom = ((maskBoard >> pos) & maskBottom): uint(32),
              lastRow = ((maskBoard >> (pos + 5)) & maskBottom): uint(32);
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

  for filter in 1..7:uint(32) {
    for pos in 0..49:uint(32) {
      maskStart[pos][filter] = totalCount;
      var filterMask = ((filter&1)<<1) | ((filter&6)<<(4-(evenRowsLookup[pos]&1))),
          pMask = maskStart[pos][0];
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
proc compareSolution(board: [] uint(8), minSolution: [] uint(8),
                     maxSolution: [] uint(8)) {

  for i in 0..49 {
    if board[i] < minSolution[i] {
      minSolution = board;
      break;
    } else if board[i] > minSolution[i] then break;
  }
  for i in 0..49 {
    if board[i] > maxSolution[i] {
      maxSolution = board;
      break;
    } else if board[i] < maxSolution[i] then break;
  }
}

//
// Pretty printed output of board
//
proc printBoard(board: [] uint(8)) {
  for i in 0..49 {
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
proc recordSolution(currentSolution: [] uint(32)) {
  var board: [0..49] uint(8),
      flipBoard: [0..49] uint(8),
      mask, pos, currentBit, b1: uint(32),
      count: uint(32),
      piece: uint(8);

  for i in 0..9 {
    mask = currentSolution[i];
    piece = ctz(mask >> 22: uint(32)): uint(8);
    mask &= maskBottom;
    b1 |= mask;
    while mask {
      currentBit = mask & (-(mask: int(32))): uint(32);
      count = ctz(currentBit): uint(32);
      board[count+pos] = piece;
      flipBoard[49-count-pos] = piece;
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
// DIY sync record that outperforms native sync vars
//
record Spinlock {
  var l: atomic bool;

  proc lock() {
    while l.testAndSet() != false do chpl_task_yield();
  }

  proc unlock() {
    l.write(false);
  }
}

var recordLock: Spinlock;

//
// Recursively add pieces to the board, and check solution when filled.
//
proc searchLinear(in board: uint(32), in pos: uint(32), in used: uint(32),
                  in placed: uint(32), currentSolution: [] uint(32)) {
  var count, evenRows, oddRows, leftBorder, rightBorder,
      s1, s2, s3, s4, s5, s6, s7, s8: uint(32);
  if placed == 10 {
    recordLock.lock();
    recordSolution(currentSolution);
    recordLock.unlock();
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

    var mask: uint(32),
        currentMask = maskStart[pos][f];

    while allMasks[currentMask] {

      while allMasks[currentMask] & boardAndUsed do
        currentMask += 1;

      if allMasks[currentMask] {
        mask = allMasks[currentMask];
        currentSolution[placed] = mask;
        searchLinear(board | (mask & maskBottom), pos, used | (mask & maskUsed),
                     placed+1, currentSolution);
        currentMask += 1;
      }
    }
  }
}

//
// Wrapper to setup initial call to recursive searchLinear
//
proc searchLinearHelper(in board: uint(32), in pos: uint(32),
                        in used: uint(32), in placed: uint(32),
                        in firstPiece: uint(32), in mask: uint(32)) {
  var currentSolution: [0..9] uint(32);
  currentSolution[0] = firstPiece;
  currentSolution[1] = mask;
  begin searchLinear(board, pos, used, placed, currentSolution);
}


//
// Add initial piece to board, then fire off remaining searches in parallel
//
proc searchParallel(in board: uint(32), in pos: uint(32), in used: uint(32),
                    in placed: uint(32), in firstPiece: uint(32)) {

  var count = ctz(~board): uint(32);
  pos += count;
  board >>= count;

  const boardAndUsed = board | used;
  var currentMask = maskStart[pos][0],
      mask: uint(32);

  if placed == 0 {
    while allMasks[currentMask] {
      if allMasks[currentMask] {
        mask = allMasks[currentMask];
        currentMask += 1;
        searchParallel(board | (mask & maskBottom), pos,
                       used | (mask & maskUsed), placed + 1, mask);
      }
    }
  } else {   // placed == 1
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

  sync searchParallel(0,0,0,0,0);

  writef("%i solutions found\n\n", solutions);
  printBoard(minSolution);
  printBoard(maxSolution);
}
