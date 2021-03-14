class C { }

class D1 : C {
  var x : int;
}

class D2 : C {
  var x : real;
}

var c1 : borrowed C = new borrowed D1();
var c2 : borrowed C = new borrowed D2();

writeln(c1);
writeln(c2);

proc foo(c : borrowed C) {
  var y = c.x;
  writeln(y);
}

foo(c1);
foo(c2);
