config const verbose = false;
config const iters = 100;
config const n = 10000;

proc f() {
  var D = {1..n};
  var A: [D] real;

  cobegin with (ref A, ref D) {
    D = {1..5};
    A = 3.14;
  }
  if verbose then
    writeln(A);
}

for i in 1..iters do
  f();
