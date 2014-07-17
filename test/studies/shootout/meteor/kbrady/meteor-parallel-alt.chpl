/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Kyle Brady
 * based upon the C++ implementation by Stefan Westen which states it was
 * loosely based on Ben St. John's and Kevin Barnes' implementation
 */

use BitOps;

const pieceCount: int(32) = 10;
const pieces: [0..9][0..4][0..1] int(32) = [
  [[0:int(32), 0:int(32)], [1:int(32), 0:int(32)], [2:int(32), 0:int(32)], [3:int(32), 0:int(32)], [3:int(32), 1:int(32)]],
  [[0:int(32), 0:int(32)], [0:int(32), 1:int(32)], [-2:int(32), 2:int(32)], [-1:int(32), 2:int(32)], [-3:int(32), 3:int(32)]],
  [[0:int(32), 0:int(32)], [1:int(32), 0:int(32)], [2:int(32), 0:int(32)], [-1:int(32), 1:int(32)], [-1:int(32), 2:int(32)]],
  [[0:int(32), 0:int(32)], [1:int(32), 0:int(32)], [2:int(32), 0:int(32)], [1:int(32), 1:int(32)], [1:int(32), 2:int(32)]],
  [[0:int(32), 0:int(32)], [0:int(32), 1:int(32)], [1:int(32), 1:int(32)], [-1:int(32), 2:int(32)], [1:int(32), 2:int(32)]],
  [[0:int(32), 0:int(32)], [1:int(32), 0:int(32)], [-2:int(32), 1:int(32)], [-1:int(32), 1:int(32)], [0:int(32), 1:int(32)]],
  [[0:int(32), 0:int(32)], [1:int(32), 0:int(32)], [0:int(32), 1:int(32)], [-1:int(32), 2:int(32)], [-1:int(32), 3:int(32)]],
  [[0:int(32), 0:int(32)], [2:int(32), 0:int(32)], [-1:int(32), 1:int(32)], [0:int(32), 1:int(32)], [1:int(32), 1:int(32)]],
  [[0:int(32), 0:int(32)], [0:int(32), 1:int(32)], [0:int(32), 2:int(32)], [1:int(32), 2:int(32)], [1:int(32), 3:int(32)]],
  [[0:int(32), 0:int(32)], [0:int(32), 1:int(32)], [0:int(32), 2:int(32)], [-1:int(32), 3:int(32)], [0:int(32), 3:int(32)]]
];

var allMasks: [0..8191] uint(32);
var maskStart: [0..49][0..7] uint(32);

var minSolution: [0..49] uint(8);
var maxSolution: [0..49] uint(8);
var solutions: uint(32);

var evenRowsLookup: [0..49] uint(32);
var leftBorderLookup: [0..49] uint(32);

proc goodPiece(mask: uint(32), pos: uint(32)): bool {
   var isGood: bool = true;
   const evenRows: uint = 0xf07c1f07c1f07c1f;
   const oddRows: uint = ~evenRows;
   const leftBorder: uint = 0x1084210842108421;
   const rightBorder: uint = leftBorder >> 1;
   var a, b, aOld, s1, s2, s3, s4, s5, s6, s7, s8: uint;

   b = (mask:uint << pos) | 0xFFFC000000000000;

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

proc initialise() {
  for i in 0..49 {
    evenRowsLookup[i] = (0xF07C1F07C1F07C1F >> i):uint(32);
    leftBorderLookup[i] = (0x1084210842108421 >> i):uint(32);
  }

  var totalCount: uint(32);
  var x: [0..4] int(32);
  var y: [0..4] int(32);
  for yBase in 2..3:int(32) {
    for xBase in 0..4:int(32) {
      const pos: uint(32) = (xBase+5*yBase):uint(32);
      maskStart[pos][0] = totalCount;
      for piece in 0..pieceCount-1 {
        for j in 0..4 {
          x[j] = pieces[piece][j][0];
          y[j] = pieces[piece][j][1];
        }

        for currentRotation in 0..11 {
          if piece != 3 || (currentRotation/3) % 2 == 0 {
            var minX: int(32) = x[0];
            var minY: int(32) = y[0];
            for i in 1..4 {
              if y[i] < minY || (y[i] == minY && x[i] < minX) {
                minX = x[i];
                minY = y[i];
              }
            }

            var mask: uint(32);
            var fit: bool = true;

            for i in 0..4 {
              var nX: int(32) = (x[i]-minX+(xBase-yBase/2)) + (y[i]-minY+yBase)/2;
              var nY: int(32) = y[i]-minY+yBase;
              if nX >= 0 && nX < 5 {
                var numBits: int(32) = nX-xBase+5*(nY-yBase);
                mask |= (1:int(32)<<numBits);
              } else {
                fit = false;
              }
            }
            if fit {
              if goodPiece(mask, pos) {
                allMasks[totalCount] = mask|(1:int(32)<<(piece+22));
                totalCount += 1;
              }
            }
          }

          for i in 0..4 {
            const xnew: int(32) = x[i]+y[i];
            const ynew: int(32) = -x[i];
            x[i] = xnew;
            y[i] = ynew;
            if currentRotation == 5 {
              const xnew:int(32) = x[i]+y[i];
              const ynew:int(32) = -y[i];
              x[i] = xnew;
              y[i] = ynew;
            }
          }
        }
      }
      allMasks[totalCount] = 0;
      totalCount += 1;
    }
  }

   // copy rows 2 and 3 to other rows

  for yBase in 0..9:int(32) {
    if yBase!=2 && yBase!=3 {
      for xBase in 0..4:int(32) {
        const pos: uint(32) = (xBase+5*yBase):uint(32);
        const origPos: int(32) = xBase+5*(yBase%2+2);
        maskStart[pos][0] = totalCount;
        var pMask: uint(32) = maskStart[origPos][0];
        const bottom: uint(32) = ((0xFFFC000000000000>>pos) & 0x003FFFFF):uint(32);
        const lastRow: uint(32) = ((0xFFFC000000000000>>(pos+5)) & 0x003FFFFF):uint(32);
        while allMasks[pMask] {
          var mask: uint(32) = allMasks[pMask];
          pMask += 1;
          if (mask & bottom) == 0 {
            if yBase == 0 || ((mask & lastRow) != 0) {
              if !goodPiece(mask&0x003FFFFF, pos) {
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
      var filterMask: uint(32);
      filterMask = ((filter&1)<<1) | ((filter&6)<<(4-(evenRowsLookup[pos]&1)));
      var pMask: uint(32) = maskStart[pos][0];
      while allMasks[pMask] {
        const mask:uint(32) = allMasks[pMask];
        if (mask&filterMask) == 0 {
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

proc compareSolution(board: [] uint(8), minSolution: [] uint(8),
                     maxSolution: [] uint(8)) {
  var i, j: int(32);

  for i in 0..49 {
    if board[i] < minSolution[i] {
      for j in 0..49 {
        minSolution[j] = board[j];
      }
      break;
    } else if board[i] > minSolution[i] {
      break;
    }
  }
  for i in 0..49 {
    if board[i] > maxSolution[i] {
      for j in 0..49 {
        maxSolution[j] = board[j];
      }
      break;
    } else if board[i] < maxSolution[i] {
      break;
    }
  }
}

proc printBoard(board: [] uint(8)) {
  for i in 0..49 {
    writef("%i ", board[i]);
    if i%5 == 4 {
      writeln();
      if (i&1) == 0 {
        write(" ");
      }
    }
  }
  writeln();
}

proc recordSolution(currentSolution: [] uint(32)) {
  var board: [0..49] uint(8);
  var flipBoard: [0..49] uint(8);
  var mask, pos, currentBit, b1 : uint(32);
  var piece, count : uint(32);
  for i in 0..9 {
    mask = currentSolution[i];
    piece = ctz(mask>>22:uint(32)):uint(32);
    mask &= 0x003FFFFF;
    b1 |= mask;
    while mask {
      currentBit = mask&(-(mask:int(32))):uint(32);
      count = ctz(currentBit):uint(32);
      board[count+pos] = piece:uint(8);
      flipBoard[49-count-pos] = piece:uint(8);
      mask ^= currentBit;
    }
    count = ctz(~b1);
    pos += count;
    b1 >>= count;
  }
  if solutions == 0 {
    for i in 0..49 {
      minSolution[i] = board[i];
      maxSolution[i] = board[i];
    }
  } else {
    compareSolution(board, minSolution, maxSolution);
    compareSolution(flipBoard, minSolution, maxSolution);
  }

  solutions += 2;
}

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

proc searchLinear(in board: uint(32), in pos: uint(32), in used: uint(32),
                  in placed: uint(32), currentSolution: [] uint(32)) {
  var count : uint(32);
  var evenRows, oddRows, leftBorder, rightBorder: uint(32);
  var s1, s2, s3, s4, s5, s6, s7, s8: uint(32);
  if placed == 10 {
    recordLock.lock();
    recordSolution(currentSolution);
    recordLock.unlock();
  } else {
    evenRows = evenRowsLookup[pos];

    oddRows = ~evenRows;

    leftBorder = leftBorderLookup[pos];
    rightBorder = leftBorder>>1;

    s1 = (board << 1) | leftBorder;
    s2 = (board >> 1) | rightBorder;
    s3 = (board << 4) | ((1<<4)-1) | rightBorder;
    s4 = (board >> 4) | leftBorder;
    s5 = (board << 5) | ((1<<5)-1);
    s6 = (board >> 5);
    s7 = (board << 6) | ((1<<6)-1) | leftBorder;
    s8 = (board >> 6) | rightBorder;

    if ~board&s1&s2&s5&s6 & ((evenRows&s4&s7) | (oddRows&s3&s8)) {
      return;
    }

    count = ctz(~board);
    pos += count;
    board >>= count;

    const f: uint(32) = ((board>>1)&1) | ((board >> (4-(evenRowsLookup[pos] & 1)))&6);
    const boardAndUsed = board|used;

    var mask: uint(32);
    var currentMask: uint(32) = maskStart[pos][f];

    while allMasks[currentMask] {
      while allMasks[currentMask] & boardAndUsed {
        currentMask += 1;
      }
      if allMasks[currentMask] {
        mask = allMasks[currentMask];
        currentSolution[placed] = mask;
        searchLinear(board|(mask & 0x003FFFFF:uint(32)), pos,
                     used|(mask & 0xFFC00000:uint(32)), placed+1,
                     currentSolution);
        currentMask += 1;
      }
    }
  }
}

proc searchLinearHelper(in board: uint(32), in pos: uint(32),
                        in used: uint(32), in placed: uint(32),
                        in firstPiece: uint(32), in mask: uint(32)) {
  var currentSolution: [0..9] uint(32);
  currentSolution[0] = firstPiece;
  currentSolution[1] = mask;
  begin searchLinear(board, pos, used, placed, currentSolution);
}


proc searchParallel(in board: uint(32), in pos: uint(32), in used: uint(32),
                    in placed: uint(32), in firstPiece: uint(32)) {
  var count: uint(32);

  count = ctz(~board):uint(32);
  pos += count;
  board >>= count;

  const boardAndUsed: uint(32) = board|used;
  var currentMask: uint(32) = maskStart[pos][0];
  var mask: uint(32);

  if placed == 0 {
    while allMasks[currentMask] {
      if allMasks[currentMask] {
        mask = allMasks[currentMask];
        currentMask += 1;
        searchParallel(board|(mask&0x003FFFFF), pos,
                       used|(mask&0xFFC00000:uint(32)), placed+1, mask);
      }
    }
  } else {   // placed == 1
    while allMasks[currentMask] {
      while allMasks[currentMask] & boardAndUsed {
        currentMask += 1;
      }
      if allMasks[currentMask] {
        mask = allMasks[currentMask];
        currentMask += 1;
        searchLinearHelper(board|((mask&0x003FFFFF)), pos,
                           used|(mask&0xFFC00000:uint(32)),  placed+1,
                           firstPiece, mask);
      }
    }
  }
}


proc main(args: [] string): int
{
  if args.size > 2 then
    return 1;

  initialise();

  sync searchParallel(0,0,0,0,0);

  writef("%i solutions found\n\n", solutions);
  printBoard(minSolution);
  printBoard(maxSolution);

  return 0;
}
