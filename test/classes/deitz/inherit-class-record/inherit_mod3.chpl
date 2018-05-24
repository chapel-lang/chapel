class C { }

class D1 : C {
  var x : int;
}

class D2 : C {
  var x : real;
}

proc foo(c : C) {
  writeln(c.x);
}

var c : C;
c = new borrowed D1();
foo(c);
c = new borrowed D2();
foo(c);
