class C {
  var X: [1..3] real;

  proc init(IX: [1..3] real) {
    X = IX;
  }
}

var A: [1..10] real;
var myC = new unmanaged C(A[1..3]);

A = 1.0;

writeln(myC);
writeln(A);

delete myC;
