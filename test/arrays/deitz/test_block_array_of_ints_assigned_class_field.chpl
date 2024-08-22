use BlockDist;

const D = {1..4} dmapped new blockDist({1..4});

class C {
  var i: int = -1;
}

var cOwn = new owned C();
var c = cOwn.borrow();

var A: [D] int = c.i;

writeln(A);

c.i = -2;

forall e in A do
  e = c.i;

writeln(A);
