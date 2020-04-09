config const n = 1000000;
config const loop = 1;

var A: [1..n][1..3] real;
var res: [1..n] real;

if loop == 1 {
  // serialize reduction of forall loop with a serial statement
  forall i in 1..n do
    serial do res[i] = + reduce [j in 1..3] A[i][j];
} else if loop == 2 {
  // serialize reduction by reducing a serial iterator
  forall i in 1..n do
    res[i] = + reduce for j in 1..3 do A[i][j];
} else {
  // serialize reduction of an array slice with a serial statement
  forall i in 1..n do
    serial do res[i] = + reduce A[i][1..3];
}
