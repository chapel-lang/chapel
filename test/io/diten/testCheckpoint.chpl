use BlockDist;
use checkpoint;

config const filename = "myBlockArr.bin";

config const m = 5, n = 4;

var Dom = newBlockDom({1..m, 1..n});

var A: [Dom] int;

for (i,j) in Dom {
  A[i,j] = i*10 + j;
}

checkpointArray(filename, A);
writeln("Wrote checkpoint file");
A = 0;
writeln("Zeroed 'A'");

readArrayCheckpoint(filename, A);
writeln("Read checkpoint file");
writeln(A);
