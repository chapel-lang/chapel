use Set;

class C { var x: int; }

var myC = new C?(45);
var myC2 = new C?(33);

var A: [1..10] owned C?;

A[3] = myC;
A[5] = myC;
A[7] = myC2;

record R {
  var idx: int;
}

operator ==(lhs: R, rhs: R) {
  return A[lhs.idx] == A[rhs.idx];
}

var s: set(R);

var myR = new R(3);
var myR2 = new R(5);
var myR3 = new R(7);

s.add(myR);
s.add(myR2);
s.add(myR3);

writeln(s);
