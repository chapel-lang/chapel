config const n = 4;

const D = [1..n, 1..n];

var A: [D] real = [(i,j) in D] i + j/10.0;

writeln("A is: ", A);


var str = foo[ A[1, ..], A[.., 1] ];

writeln(str);

def foo(r: real, c: real) {
  return "aRow is: " + r + "; aCol is: " + c + "\n";
}
