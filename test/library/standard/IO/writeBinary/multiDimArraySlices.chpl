use IO, BlockDist;

config const useCol = false;
config param useBlock = false;

var Dom = if useBlock then blockDist.createDomain({1..10, 1..10})
                      else {1..10, 1..10};
var Slice = if useCol then {1..10, 2..2} else {2..2, 1..10};

var A: [Dom] real = [(i,j) in Dom] i * 100 + j/100.0;

writeln(A);

var outfile = open("out.bin", ioMode.cw);
outfile.writer(locking=false).writeBinary(A[Slice]);
outfile.close();

var B: [Dom] real;

var infile = open("out.bin", ioMode.r);
infile.reader(locking=false).readBinary(B[Slice]);
infile.close();

writeln(B);

const equalIn = + reduce (A[Slice] == B[Slice]);
const equalZero = + reduce (B == 0);

if equalIn + equalZero != Dom.size then
  halt("Mismatch between A and B");
else
  writeln("Matched!");
