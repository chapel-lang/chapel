config const n = 10000;
config const trials = 100;
config const verbose = false;

var A: [1..n] int;

for i in 1..trials {

  coforall i in 1..n {
    A(i) += 1;
  }

  for i in 1..n {
    if A(i) != 1 {
      writeln(A);
      halt("error detected");
    }
  }

  if verbose then
    writeln("trial ", i, " success");

  A = 0;
}
writeln("success");
