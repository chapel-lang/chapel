record D {
  var d : int;
}

record C {
  var c : D;
}

record B {
  var b : C;
}

var a : B;

a.b.c.d = 2;
writeln(a.b.c.d);
