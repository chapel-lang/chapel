class C : D {
  var x : integer = 1;
}

class D : C {
  var y : integer = 1;
}

var c : C = C();

writeln(c);
