class C { }

class D1 : C {
  var x : int;
}

class D2 : C {
  var x : real;
}

var c1obj = new D1(); var c1 : borrowed C = c1obj.borrow();
var c2obj = new D2(); var c2 : borrowed C = c2obj.borrow();

writeln(c1);
writeln(c2);

proc foo(c : borrowed C) {
  var y = c.x;
  writeln(y);
}

foo(c1);
foo(c2);
