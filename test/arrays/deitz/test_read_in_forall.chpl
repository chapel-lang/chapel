use IO;

config const verbose = false;

var A: [0..8, 0..8] int = [i in {0..8, 0..8}] read(int);

if verbose then
  writeln(A);

writeln(+ reduce A);
