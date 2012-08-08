config const verbose = false;
config const iters = 100;

proc f() {
  var D = {1..10000};
  var A: [D] real;

  cobegin {
    A = 3.14;
    D = {1..5};
  }
  if verbose then
    writeln(A);
}

for i in 1..iters do
  f();
