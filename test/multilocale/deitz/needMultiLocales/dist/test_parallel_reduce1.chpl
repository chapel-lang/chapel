use BlockDist;

config const tasksPerLocale=2, m=8, iters=100, verbose=false;

const Dist = new dmap(new Block(rank=1, boundingBox={1..m},
                                dataParTasksPerLocale=tasksPerLocale));
const Dom: domain(1) dmapped Dist = {1..m};
var A, B: [Dom] real;

var s$: sync int = 1;

forall (a,b) in zip(A,B) {
  const ss = s$.readFE();
  a = ss;
  b = ss + 1;
  s$.writeEF(ss + 2);
}

if verbose {
  writeln(A);
  writeln(B);
}

for i in 1..iters do
  write(max reduce [(a,b) in zip(A,B)] abs(a + b));
writeln();
