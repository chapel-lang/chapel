const grid = {1..100}; // 1D domain
const alpha = 5;       // some number
var A, B, C: [grid] real;  // local real-type arrays on this domain
B = 2; C = 3;
forall (a,b,c) in zip(A,B,C) do // parallel loop
  a = b + alpha*c;  // example of data parallelism on a single locale
writeln(A);
