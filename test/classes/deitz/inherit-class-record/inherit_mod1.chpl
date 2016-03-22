class C { }

class D1 : C {
  var x : int;
}

class D2 : C {
  var x : real;
}

var c1 : C = new D1();
var c2 : C = new D2();

writeln(c1);
writeln(c2);

proc foo(c : C) {
  writeln(c.x);
}

foo(c1);
foo(c2);
