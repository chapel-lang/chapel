use IO;

config const numSteps = 2, debug = false;

var decoder: string;
readLine(decoder);

var blank: string;
readLine(blank);

var D: domain(2) = {-(2*numSteps+1)..2*numSteps+1, 1..0};
var A, B: [D] bool;

var line: string;
var numLines = 0;
while readLine(line) {
  numLines += 1;
  D = {-(2*numSteps+1)..numLines+2*numSteps+1, -(2*numSteps+1)..line.size+2*numSteps+1};
  for (ch,col) in zip(line, 0..) do
    A[numLines, col] = (ch == "#");
}

const DInner = D.expand(-1);

for i in 1..numSteps {
  if debug then
    writeImage(A);

  forall rc in D {
    var num = 0;
    for off in {-1..1, -1..1} {
      num <<= 1;
      if D.contains(rc+off) then
        num |= A[rc+off];
      else
        num |= A[rc];
    }
    B[rc] = decoder[num] == "#";
  }
  B <=> A;
}

writeImage(A);
writeln(+ reduce A);

proc writeImage(A) {
  for r in A.dim(0) {
    for c in A.dim(1) do
      write(if A[r,c] then "#" else ".");
    writeln();
  }
}
