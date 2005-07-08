record C {
  var c : integer;
}

record B {
  var b : C;
}

var a : B;

a.b.c = 2;
writeln(a.b.c);
