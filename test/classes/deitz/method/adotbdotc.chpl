record C {
  var c : int;
}

record B {
  var b : C;
}

var a : B;

a.b.c = 2;
writeln(a.b.c);
