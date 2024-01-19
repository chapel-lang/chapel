proc negate(ref x: real) {
  x = -x;
}

var pi = 3.1415;
negate(pi);
writeln(pi, "\n");

var A = [1.2, 3.4, 5.6];
negate(A);
writeln(A, "\n");


forall a in A do
  negate(a);

writeln(A, "\n");

var A2: [1..3, 1..3] real
      = forall (i,j) in {1..3, 1..3} do i + j/10.0;

negate(A2);
writeln(A2, "\n");

forall a2 in A2 do
  negate(a2);
writeln(A2, "\n");

// Tests of multi-argument promotion
proc sortTwo(ref x: real, ref y: real) {
  if x > y then
    x <=> y;
}

var B = [2.3, 3.4, 5.6],
    C = [0.2, 4.6, 1.3];

sortTwo(B, C);

writeln(B, "\n", C, "\n");

B = [2.3, 3.4, 5.6];
C = [0.2, 4.6, 1.3];

forall (b, c) in zip(B, C) do
  sortTwo(b, c);

writeln(B, "\n", C, "\n");

// Tests of promoting some, but not all, arguments
proc maybeCopy(ref x: real, y: real, b: bool) {
  if b then
    x = y;
}

var r = 0.0, s = 1.2;

maybeCopy(r, s, false);
writeln(r);
maybeCopy(r, s, true);
writeln(r);

A = 0.0;
B = [1.2, 3.4, 5.6];
var Mask = [true, false, true];

maybeCopy(A, 1.2, true);
writeln(A);

A = 0.0;
forall a in A do
  maybeCopy(a, 1.2, true);
writeln(A);

A = 0.0;
maybeCopy(A, B, true);
writeln(A);

A = 0.0;
forall (a, b) in zip(A, B) do
  maybeCopy(a, b, true);
writeln(A);

A = 0.0;
maybeCopy(A, 1.2, Mask);
writeln(A);

A = 0.0;
forall (a, m) in zip(A, Mask) do
  maybeCopy(a, 1.2, m);
writeln(A);

A = 0.0;
maybeCopy(A, B, Mask);
writeln(A);

A = 0.0;
forall (a, b, m) in zip(A, B, Mask) do
  maybeCopy(a, b, m);
writeln(A);

config param testError = false, testError2 = false;

if testError {
  var r = 0.0;
  maybeCopy(r, B, Mask);
 if testError2 {
  forall (b, m) in zip(B, Mask) do
    maybeCopy(r, b, m);
} }

// safe since only one mask value will modify 'r'
Mask = [false, false, true];
forall (b, m) in zip(B, Mask) with (ref r) do
  maybeCopy(r, b, m);
writeln(r);

// promoting operators and functions
A = [1.0, 4.0, 9.0];
B = sqrt(A);
writeln(B);

forall (b, a) in zip(B, A) do
  b = sqrt(a);
writeln(B);

B = A + A;
writeln(B);

forall (b, a) in zip(B, A) do
  b = a + a;
writeln(B);

// promotion vs. whole-array operations
B = [1.2, 3.4, 5.6];
C = A + 2*B;
writeln(C);

const T = 2*B;
C = A + T;
writeln(C);

forall (c, a, b) in zip(C, A, B) do
  c = a + 2*b;
writeln(C);

// these are race-y (but safe for this test since all values are 0.0):
var V: [1..10] real;
V[2..9] = (V[1..8] + V[3..10]) / 2.0;
writeln(V);

forall (v1, v2, v3) in zip(V[2..9], V[1..8], V[3..10]) do
  v1 = (v2 + v3)/2.0;
writeln(V);

forall i in 2..9 do
  V[i] = (V[i-1] + V[i+1]) / 2.0;
writeln(V);

// these avoid the race:

V = [1.0, 2.0, 4.0, 8.0, 16.0, 32.0, 64.0, 128.0, 256.0, 512.0];
const T2 = V[1..8] + V[3..10];
V[2..9] = T2 / 2.0;
writeln(V);

V = [1.0, 2.0, 4.0, 8.0, 16.0, 32.0, 64.0, 128.0, 256.0, 512.0];
const V2 = (V[1..8] + V[3..10]) / 2.0;
V[2..9] = V2;
writeln(V);

V = [1.0, 2.0, 4.0, 8.0, 16.0, 32.0, 64.0, 128.0, 256.0, 512.0];
proc arrayAdd(X: [] real, Y: [] real) {
  var Z = X + Y;
  return Z;
}

V[2..9] = arrayAdd(V[1..8], V[3..10]) / 2.0;
writeln(V);

// promoting using ranges and domains:

A = 0.0;
Mask = [true, false, true];
maybeCopy(A, 1..6 by 2, true);
writeln(A);

A = 0.0;
maybeCopy(A, A.domain, true);
writeln(A);

A = 0.0;
maybeCopy(A, forall i in 1..3 do 2*i + 0.5, true);
writeln(A);

// evaluating a call only once in multi-argument promotion

writeln();
proc computeMask() {
  writeln("computing mask...");
  return true;
}

A = 0.0;
B = [1.2, 3.4, 5.6];
maybeCopy(A, B, computeMask());
writeln(A, "\n");

A = 0.0;
var tmp = computeMask();
forall (a, b) in zip(A, B) do
  maybeCopy(a, b, tmp);
writeln(A);
