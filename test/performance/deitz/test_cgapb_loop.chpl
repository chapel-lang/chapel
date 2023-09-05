param N = 1024;
param ITERS = 200;

proc main() {
  const D = {0..N-1, 0..N-1};
  var A, B, C: [D] int;
  [(i,j) in D with (ref A)] A(i,j) = i+j;
  [(i,j) in D with (ref B)] B(i,j) = i*j;
  for i in 1..ITERS do
    [(i,j) in D with (ref C)] C(i,j) = A(i,j) + B(i,j);
  writeln(C(51,51));
}
