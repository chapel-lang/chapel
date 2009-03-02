use BlockDist;

config const tasksPerLocale=2, m=8:int(64), iters=100, verbose=false;

const Dist = new Block1D(bbox=[1..m], tasksPerLocale=tasksPerLocale);
const Dom: domain(1, int(64)) distributed Dist = [1..m];
var A, B: [Dom] real;

var s$: sync int = 1;

forall (a,b) in (A,B) {
  const ss = s$;
  a = ss;
  b = ss + 1;
  s$ = ss + 2;
}

if verbose {
  writeln(A);
  writeln(B);
}

for i in 1..iters do
  write(max reduce [(a,b) in (A,B)] abs(a + b));
writeln();
