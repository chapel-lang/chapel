use BlockDist;

const D = {1..4} dmapped Block({1..4});

class C {
  var i: int;
}

var cOwn = new owned C();
var c = cOwn.borrow();

forall i in D {
  writeln(c.i);
}

var A: [D] int;

forall e in A {
  writeln(c.i);
}
