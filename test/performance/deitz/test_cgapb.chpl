param N = 1024;
param ITERS = 200;

proc main() {
  const D = {0..N-1, 0..N-1};
  var A, B, C: [D] int;
  [(i,j) in D] A(i,j) = i+j;
  [(i,j) in D] B(i,j) = i*j;
  for i in 1..ITERS do
    C = A + B;
  writeln(C(51,51));
}
