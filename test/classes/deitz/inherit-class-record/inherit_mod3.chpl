class C { }

class D1 : C {
  var x : int;
}

class D2 : C {
  var x : real;
}

proc foo(c : borrowed C) {
  writeln(c.x);
}

var c : borrowed C?;
c = (new owned D1()).borrow();
foo(c!);
c = (new owned D2()).borrow();
foo(c!);
