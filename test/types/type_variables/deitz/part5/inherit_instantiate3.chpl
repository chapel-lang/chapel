class C {
  var x : int;
}

class D : C {
  param p : int;
  var y : real;
}

var d = (new owned D(p=2)).borrow();

writeln(d);

proc foo(d : borrowed D) {
  for param i in 1..d.p do
    writeln(i);
}

foo(d);
