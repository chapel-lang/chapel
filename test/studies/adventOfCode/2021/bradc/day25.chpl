use IO;

enum cuc { empty=0, east, south };
use cuc;

var D: domain(2);
var A: [D] cuc;
var numrows, numcols = 0;

var line: string;
while readLine(line) {
  line = line.strip();
  numcols = line.size;
  D = {0..numrows, 0..<numcols};
  for (ch, i) in zip(line, 0..) do
    A[numrows, i] = chToCuc(ch);
  numrows += 1;
}

config const debug = numcols < 80;

writeCucs(A);
var numSteps = 0;

do {
  numSteps += 1;
  var someoneMoved = false;

  var B = A;
  forall ij in D with (ref someoneMoved) {
    if A[ij] == east {
      const target = (ij + (0,1))%(numrows, numcols);
      if A[target] == empty {
        B[ij] = empty;
        B[target] = east;
        someoneMoved = true;
      }
    }
  }

  A = B;

  forall ij in D with (ref someoneMoved) {
    if B[ij] == south {
      const target = (ij + (1,0))%(numrows, numcols);
      if B[target] == empty {
        A[ij] = empty;
        A[target] = south;
        someoneMoved = true;
      }
    }
  }
  if debug then
    writeCucs(A);
} while someoneMoved;

writeln(numSteps);


proc chToCuc(ch: string) {
  if ch == ">" then
    return east;
  if ch == "v" then
    return south;
  return empty;
}

proc writeCucs(A) {
  for r in A.domain.dim(0) {
    for c in A.domain.dim(1) do
      select A[r,c] {
        when empty do write(".");
        when east do write(">");
        when south do write("v");
      }
    writeln();
  }
  writeln();
}
