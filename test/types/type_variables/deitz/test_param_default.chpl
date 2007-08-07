class C {
  param p: int = 4;
  var x: p*int;
}

var c = C(p=5);
writeln(c);

var c2 = C();
writeln(c2);
