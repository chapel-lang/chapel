class C {
  var x : int;
}

class D : C {
  param p : int;
  var y : real;
}

var ownD = new owned D(p=2);
var d = ownD.borrow();

writeln(d);

proc foo(d : borrowed D) {
  for param i in 1..d.p do
    writeln(i);
}

foo(d);
