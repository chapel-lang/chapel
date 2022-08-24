class C { }

class D1 : C {
  var x : int;
}

class D2 : C {
  var x : real;
}

var c1 : borrowed C = (new owned D1()).borrow();
var c2 : borrowed C = (new owned D2()).borrow();

writeln(c1);
writeln(c2);

proc foo(c : borrowed C) {
  writeln(c.x);
}

foo(c1);
foo(c2);
