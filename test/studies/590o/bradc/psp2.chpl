config const n = 4;

const D = [1..n, 1..n];

var A: [(i,j) in D] real = i + j/10.0;

writeln("A is: ", A);


foo[ A[1, ..], A[.., 1] ];


def foo(r: real, c: real) {
  writeln("aRow is: ", r, "; aCol is: ", c);
}
