// Example of the Jacobi algorithm in Chapel

param M = 4;
param N = 4;
param MAXDELTAPOSSIBLE = 1.0;
param THRESHOLD = 0.00001;

var maxdelta: real = MAXDELTAPOSSIBLE;

var outerD: domain(2) = {0..M+1, 0..N+1};
var D: domain(2) = {1..M, 1..N};
var northOfD: domain(2) = {0..0,     1..N};
var southOfD: domain(2) = {M+1..M+1, 1..N};
var westOfD : domain(2) = {1..M, 0..0};
var eastOfD : domain(2) = {1..M, N+1..N+1};

var A:[outerD] real;
var newA:[D] real;
var delta:[D] real;

// initialization of A
[e in D] A(e) = 0.0;
[e in northOfD] A(e) = 0.0;
[e in southOfD] A(e) = 1.0;
[e in westOfD]  A(e) = 0.0;
[e in eastOfD]  A(e) = 0.0;

do {
  forall (i,j) in D do
    newA(i,j) = (A(i-1,j) + A(i+1,j) + A(i,j-1) + A(i,j+1)) / 4.0;
  [e in D] delta(e) = abs( newA(e) - A(e));
  maxdelta = max reduce delta;
  [e in D] A(e) = newA(e);
} while (maxdelta > THRESHOLD);

writeln( A);
