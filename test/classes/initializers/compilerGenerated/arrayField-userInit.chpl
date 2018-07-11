use BlockDist;

class C {
  var X: [1..3] real;

  proc init(IX: [1..3] real = [0, 0, 0]) {
    X = IX;
  }
}

const D = {1..3} dmapped Block({1..3});
var A: [D] real;

var myC = new borrowed C(A);
writeln("myC is: ", myC);
